/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009  Andrey Stroganov <savthe@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "downloader.hpp"
#include "download.hpp"
#include "util.hpp"
#include "receiver.hpp"
#include "net_err.hpp"
#include "type2stream.hpp"
#include "../app.hpp"
#include "../verb.hpp"
#include "../appdefs.hpp"
#include "settings.hpp"
#include "../traf.hpp"

namespace download_details {

using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::istarts_with;
using boost::trim;
using boost::trim_copy;
using boost::bind;
using util::since;
using util::now;

using namespace download_details::dlr_state;

std::size_t Downloader::instances_ = 0;

template <typename T> void extract_ranges(std::string value, T out)
{
	if(!boost::istarts_with(value, "bytes")) return;
	value.erase(0, 7);
	boost::replace_all(value, ",", " ");
	boost::replace_all(value, "-", " ");

	std::stringstream s(value);
	file_offset_t first, last;
	while((s >> first >> last) && first <= last)
	{
		ByteRange r;
		r.set_bound(first, last);
		*out++ = r;
	}
}

void extract_locations(const std::string& value, Sources& srcs)
{	
	std::stringstream s(value);
	std::string line;
	while(std::getline(s, line, ','))
	{
		trim(line);
		const std::string uri = line.substr(0, line.find(' '));
		SourceHost host;
		if(host.FromUri(uri))
			srcs.insert(std::make_pair(host.endpoint, host));
	}
}

template <typename S, typename T> void flush(S& s, T& buffer)
{
	std::vector<char> v;
	while (s.good())
	{
		const char c = s.get();
		if(s.good()) v.push_back(c);
	}

	if(!v.empty())
		buffer.put(&v[0], v.size());
}

inline void abort(int n)
{
	throw bs::error_code(n, net_err::category());
}

Downloader::Downloader(ip::Endpoint e, Sources& s):
	socket_(new ba::ip::tcp::socket(app::io())),
	message_(16 * 1024),
	endpoint_(e),
	traf_(app::traf_in()),
	receiver_(new Receiver(*socket_, traf_)),
	buffer_(0),
	pushed_(false),
	state_(CLOSED),
	sources_(s)
{
	++instances_;
}

Downloader::Downloader(TcpSocketPtr sock, ip::Endpoint e, Sources& s):
	socket_(sock),
	message_(16 * 1024),
	endpoint_(e),
	traf_(app::traf_in()),
	receiver_(new Receiver(*socket_, traf_)),
	buffer_(0),
	pushed_(true),
	state_(IDLE),
	sources_(s)
{
	++instances_;
}

Downloader::~Downloader()
{
	--instances_;
}

void Downloader::done(const bs::error_code& err)
{
	buffer_.reset();
	Handler h;
	h.swap(handler_);
	if(!closed()) h(err);
}

void Downloader::connect(Handler h)
{
	handler_ = h;
	state_ = CONNECT;
	ba::ip::tcp::endpoint e(ba::ip::address_v4(endpoint_.address.reversed()), endpoint_.port);
	socket_->async_connect(e, bind(&Downloader::done, this, ba::placeholders::error));
}

inf::Downloader Downloader::info() const
{
	inf::Downloader i;

	i.pushed = pushed_;
	i.endpoint = endpoint_;
	i.state = state_;
	i.range = task_.range;
	i.total = traf_.total();
	i.rate = traf_.rate();
	i.q_pos = queue_.position;
	i.q_length = queue_.length;

	return i;
}

void Downloader::close()
{
	if(closed()) return;

	if(state_ == TRANSFER) receiver_->close();
	else if(socket_->is_open()) socket_->close();

	state_ = CLOSED;
}

void Downloader::headers_sent(const bs::error_code& err)
{
	if(err || closed()) done(err);
	else ba::async_read_until(*socket_, message_, "\r\n\r\n", bind(&Downloader::headers_received, this, ba::placeholders::error));
}
		
void Downloader::process_request()
{
	std::istream s(&message_);
	traf_.consume(message_.size());

	int code;
	std::string header;
	if( !(s >> header >> code) )
		abort(net_err::bad_reply);
	getline(s, header);

	//file_offset_t first = 0, last = 0, total = 0, contentLength = 0;
	file_offset_t content_length = 0;
	ByteRange offered_range;

	while(std::getline(s, header) && header != "\r")
	{
		const std::string value = trim_copy(header.substr(header.find(':') + 1));

		if(istarts_with(header, "X-Available-Ranges:"))
		{
			ranges_.clear();
			extract_ranges(value, std::back_inserter(ranges_));
		}
		
		else if(istarts_with(header, "Alt-Location:"))
			extract_locations(value, sources_);

		else if(tiger_uri_.empty() && istarts_with(header, "X-Thex-URI:"))
 		{
 			//urn:tree:tiger/:BASE32HASH
			std::size_t i = value.find("urn:tree:tiger/:");
			if(i != std::string::npos)
			{
				tiger_uri_ = "/gnutella/tigertree/v3?";
				while(i < value.size() && value[i] != '&' && value[i] != ';')
					tiger_uri_.push_back(value[i++]);
			}
		}

		else if(istarts_with(header, "Content-Length:"))
			content_length = lexical_cast<file_offset_t>(value);

		else if(istarts_with(header, "X-Queue:"))
		{
			queue_.FromString( value );
			queue_.timestamp = now();

			if(code == 503)
				abort(net_err::queued);
		}

		else if(istarts_with(header, "Content-Range:"))
		{
			file_offset_t first = 0, last = 0, total = 0;
			if(sscanf(value.c_str(), "bytes%*c%llu-%llu/%llu", &first, &last, &total) < 3 || first > last || total == 0)
				abort(net_err::bad_reply);
			offered_range.set_bound(first, last);
		}
	}
	
	if(offered_range.empty()) 
		offered_range.set(0, content_length);

	switch (code)
	{
		case 200: case 206:
		{
			if(task_.range.empty()) 
				task_.range = offered_range;

			if(task_.range != offered_range)
			{
				verb2::dl << "Host " << endpoint_ << " is forcing range " << offered_range << " instead of " << task_.range << std::endl;
				abort(net_err::forced_range);
			}

			state_ = TRANSFER;
			buffer_->range(task_.range);
			assert(task_.range.length() >= message_.size());
			const std::size_t remain = task_.range.length() - message_.size();
			flush(s, *buffer_);
			receiver_->receive(remain, *buffer_, bind(&Downloader::done, this, _1));

			break;

		}
		case 503:
 		{
			verb2::dl << "Host " << endpoint_ << " busy to upload.";
			abort(net_err::busy);
			//if(state_ == QUEUED) verb2::dl << " Waiting in queue " << qinfo_.position << " of " << qinfo_.length;
			//verb2::dl << std::endl;
			//if(state_ != QUEUED) throw std::runtime_error("Refused");
			break;
		}
		default: 
			abort(net_err::denied);
	}
}

void Downloader::fetch()
{
	state_ = CHAT;

	verb1::dl << "Downloader " << endpoint_ << " requesting " 
		<< task_.uri << " range: " << task_.range.offset() << ":" << task_.range.length() << std::endl;

	std::ostream r(&message_);
	r << "GET " << task_.uri << " HTTP/1.1" << "\r\n";

	if(!task_.accept_type.empty())
		r << "Accept: " << task_.accept_type << "\r\n";

	r << "Range: bytes=" << task_.range.first() << "-";
	if(task_.range.length()) 
		r << task_.range.last();
	r << "\r\n";

	compose_common_headers(r);
	r << "\r\n";

	traf_.consume(message_.size());

	ba::async_write(*socket_, message_, bind(&Downloader::headers_sent, this, ba::placeholders::error));
}

void Downloader::headers_received(const bs::error_code& err)
{
	try
	{
		if(err || closed()) throw err;
		
		process_request();
	}
	catch(bs::error_code& e)
	{
		done(e);
	}
}

void Downloader::compose_common_headers(std::ostream& ostr)
{
	ostr << "Host: " << endpoint_ << "\r\n";
	ostr << "Connection: Keep-Alive" << "\r\n";
	ostr << "X-Features: g2/1.0" << "\r\n";
	ostr << "User-Agent: " << app::fullname << "\r\n";
	ostr << "Listen-IP: " << app::endpoint() << "\r\n";
	ostr << "X-Queue: 0.1" << "\r\n";
	ostr << "X-Nick: " << app::settings().general.userName << "\r\n";
}

} // namespace download_details
