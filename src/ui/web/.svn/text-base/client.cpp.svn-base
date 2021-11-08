/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov <savthe@gmail.com>

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

#include <algorithm>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "client.hpp"
#include "../common/server.hpp"
#include "settings.hpp"
#include "filesystem.hpp"
#include "url.hpp"
#include "contenttype.hpp"
#include "server.hpp"
#include "verb.hpp"
#include "io.hpp"

#include "../common/query.hpp"
#include "../common/template.hpp"
#include "util.hpp"

using boost::iequals;
using boost::starts_with;
using FileSystem::Exists;
using FileSystem::FileSize;
using FileSystem::Extension;

typedef boost::shared_ptr <AsyncFile> AsyncFilePtr;

namespace ui { namespace web {

const unsigned int CONTENT_MAX_SIZE = 32 * 1024 * 1024;
const std::time_t ALIVE_TIME = 180;

Client::Client(Server* pServer):
	socket_(app::io()), 
	message_(64 * 1024),
	closed_(false),
	pServer_(pServer)
{
}

Client::~Client()
{
}

ip::Endpoint Client::GetEndpoint()
{
	ip::Endpoint e(socket_.remote_endpoint().address().to_v4().to_ulong(), socket_.remote_endpoint().port());
	e.address.reverse();
	return e;
}

void Client::Start()
{
	const ip::Address addr = GetEndpoint().address;

	bool found = false;
	BOOST_FOREACH(IpSegment s, app::settings().general.uisegments)
		found |= s.contains(addr);

	if(!found)
	{
		std::ostream r(&message_);
		r << "IP not allowed" << "\r\n\r\n";
		ba::async_write(socket_, message_, boost::bind(&Client::OnFinal, shared_from_this(), ba::placeholders::error));
	}
	else ba::async_read_until(socket_, message_, "\r\n\r\n", boost::bind(&Client::OnHeadersReceived, shared_from_this(), ba::placeholders::error));
}

void Client::OnHeadersReceived(const bs::error_code& err)
{
	if(closed_) return;

	if(!err)
	{
		try { TranslateHeaders(); }
		catch(std::exception& e)
		{
			verb1::ui << "Webui client " << GetEndpoint() << " couldn't handle request => " << e.what() << std::endl;
			DetachMe();
			return;
		}
	}
	else if(err != ba::error::operation_aborted)
	{
		verb1::ui << "Webui client " << GetEndpoint() << " => " << err.message() << std::endl;
		DetachMe();
		return;
	}
}

void Client::TranslateHeaders()
{
	fileRange_.reset();
	keepAlive_ = 0;
	unsigned int length = 0;
	content_.clear();

	std::string method, proto;
	std::istream r(&message_);

	if(!(r >> method >> object_ >> proto)) 
		throw std::runtime_error("Couldn't parse first line"); 
	if(iequals(method, "GET")) method_ = GET;
	else if(iequals(method, "POST")) method_ = POST;
	else throw std::runtime_error("Method is not supported => " + method);

	std::string line;
	r.ignore(1);
	while(getline(r, line) && line != "\r")
	{
		std::stringstream s(line);
		std::string header;
		s >> header;

		if(iequals(header, "Connection:"))
		{
			std::string value;
			s >> value;
			if(iequals(value, "keep-alive"))
			keepAlive_ = util::now() + ALIVE_TIME + 1;
		}
		else if(iequals(header, "Content-length:"))
		{
			s >> length;
		}
		else if(iequals(header, "Range:" ))
		{
			file_offset_t first = 0;
			file_offset_t last = 0;
			std::string value;
			s >> value;

			const int result = sscanf( value.c_str(), "bytes=%llu-%llu", &first, &last );
			if( result == 0 ) throw std::runtime_error( "Can't parse range" );
			if( result == 1 ) 
			{
				fileRange_.offset(first);
				fileRange_.length(0);
			}
			if( result == 2 ) fileRange_.set_bound( first, last );
		}
	}

	if(length > CONTENT_MAX_SIZE || message_.size() > length)
		throw std::runtime_error("Content length is too large");

	if(length)
	{
		while(message_.size() > 0)
			content_.push_back(r.get());
		length -= content_.size();
	}

	if(length)
	{
		buffer_.resize(length);
		ba::async_read(socket_, ba::buffer(buffer_), ba::transfer_all(), boost::bind(&Client::OnContent, shared_from_this(), ba::placeholders::error));
	}
	else HandleRequest();
}

void Client::stop()
{
	if(!closed_)
	{
		if(file_.opened()) file_.close();
		if(socket_.is_open()) socket_.close();
		closed_ = true;
	}
}

void Client::DetachMe()
{
	boost::shared_ptr<Client> me(shared_from_this());
	stop();
	pServer_->Detach(me);
}

void Client::OnContent(const bs::error_code& err)
{
	if(closed_) return;

	if(!err)
	{
		content_.insert(content_.end(), buffer_.begin(), buffer_.end());
		HandleRequest();
	}
	else if(err != ba::error::operation_aborted)
		DetachMe();
}

void Client::HandleRequest()
{
	verb2::ui << "WebUI request: " << object_ << std::endl;

	try
	{
		if(method_ == GET && starts_with(object_, "/img"))
		{
			object_.insert(0, pServer_->StyleRoot());
			ServeFile();
		}
		else if(method_ == GET && starts_with(object_, "/fetch"))
		{
			object_ = app::settings().general.complete + Url::decode(object_.substr(6));
			ServeFile();
		}
		else ProcessQuery();

	}
	catch(std::exception& e)
	{
		verb1::ui << "WebUI error: " << e.what() << std::endl;
		std::ostream r(&message_);
		r << "503 Internal Error\r\n\r\n";
		ba::async_write(socket_, message_, boost::bind(&Client::OnFinal, shared_from_this(), ba::placeholders::error));
	}
}

void Client::ServeFile()
{
	std::ostream r(&message_);

	const bool notFound = 
		object_.empty() ||
		object_.find("..") != std::string::npos ||
		!Exists(object_);

	if(notFound)
	{
		verb1::ui << "Webui requested object has not been found: " << object_ << std::endl;
		r << "404 Not Found\r\n";
		ba::async_write(socket_, message_, boost::bind(&Client::OnFinal, this, ba::placeholders::error));
		return;
	}

	file_.open(object_, fio::READ);
	if(!file_.opened())
		throw std::runtime_error("Couldn't open file " + object_);

	const file_offset_t fileSize = FileSize(object_);

	if(fileRange_.length() == 0) 
		fileRange_.length(fileSize - fileRange_.offset());

	const bool partial = fileRange_.length() != fileSize;

	if(partial) r << "HTTP/1.1 206 Partial content\r\n";
	else r << "HTTP/1.1 200 OK\r\n";
	r << "Content-Type: " << ContentType::Resolve(Extension(object_)) << "\r\n";
	r << "Content-Length: " << fileRange_.length() << "\r\n";
	if(keepAlive_) r << "Keep-Alive: " << ALIVE_TIME << "\r\n";
	r << "\r\n";
	
	ba::async_write(socket_, message_, boost::bind(&Client::OnFileTransfer, shared_from_this(), ba::placeholders::error));
}

void Client::OnFileTransfer(const bs::error_code& err)
{
	if(closed_) return;
	if(err == ba::error::operation_aborted) return;
	if(err)
	{
		verb1::ui << "Webui error during file upload => " << err.message() << std::endl;
		DetachMe();
		return;
	}

	const file_offset_t length = std::min(fileRange_.length(), file_offset_t(16 * 1024));
	if(length == 0)
	{
		OnTransferCompleted(err);
		return;
	}

	buffer_.resize(length);
	file_.read(&buffer_[0], buffer_.size(), fileRange_.offset());
	ba::async_write(socket_, ba::buffer(buffer_), boost::bind( &Client::OnFileTransfer, shared_from_this(), ba::placeholders::error));
	fileRange_.offset(fileRange_.offset() + length);
	fileRange_.length(fileRange_.length() - length);
}

void Client::ProcessQuery()
{
	if(content_.size() > 0)
	{
		object_.push_back('&');
		object_.insert(object_.end(), content_.begin(), content_.end());
	}

	if(!object_.empty()) object_.erase(0, 1);

	if(object_.empty()) object_ = StaticName::toString(ui::Tags::home);

	ui::Query q;
	
	std::string::size_type qSepPos = object_.find('?');
	if(qSepPos != std::string::npos)
		object_[qSepPos] = '&';

//	std::cout << object_ << std::endl;
	q.FromString(object_, '&');

//	const std::string root = System::GetWebui()->StyleRoot();
//	if(root.empty()) throw std::runtime_error("WARNING: Couldn't find webui style folder. Please set valid style name in sharelin.conf");

	std::string result;
	try { pServer_->ProcessQuery(q, result); }
	catch(std::exception& e)
	{
		result = "Couldn't handle command: " + object_ + " Reason: " + e.what();
	}

	content_.clear();
	content_.reserve(result.size());
	std::copy(result.begin(), result.end(), std::back_inserter(content_));

	std::ostream r(&message_);
	r << "HTTP/1.1 200 OK\r\n";
	r << "Content-Type: text/html\r\n";
	r << "Content-Length: " << content_.size() << "\r\n";
	if(keepAlive_) 
		r << "Keep-Alive: " << ALIVE_TIME << "\r\n";
	r << "\r\n";

	ba::async_write(socket_, message_, boost::bind(&Client::OnPageHeaderSent, this, ba::placeholders::error));
}

void Client::OnFinal(const bs::error_code& err)
{
	if(closed_) return;

	if(err != ba::error::operation_aborted)
		DetachMe();
}

void Client::OnPageHeaderSent(const bs::error_code& err)
{
	if(closed_) return;

	if(!err)
	{
		if(content_.empty()) OnTransferCompleted(err);
		else ba::async_write(socket_, ba::buffer(content_), boost::bind(&Client::OnTransferCompleted, this, ba::placeholders::error));
	}
	else if(err != ba::error::operation_aborted)
		DetachMe();
}

void Client::OnTransferCompleted(const bs::error_code& err)
{
	if(closed_) return;

	if(!err)
	{
		if(util::now() > keepAlive_)
		{
			DetachMe();
			return;
		}

		ba::async_read_until(socket_, message_, "\r\n\r\n", boost::bind(&Client::OnHeadersReceived, shared_from_this(), ba::placeholders::error));
	}
	else if(err != ba::error::operation_aborted)
		DetachMe();
}

} // namespace web
} // namespace ui
