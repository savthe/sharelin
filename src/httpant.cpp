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

#include <stdexcept>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "httpant.hpp"
#include "appdefs.hpp"

using boost::posix_time::seconds;
using boost::posix_time::hours;
using boost::istarts_with;
using boost::lexical_cast;
using boost::trim_copy;
using boost::bind;

HttpAnt::HttpAnt(ba::io_service& io, unsigned int sz):
	socket_(io),
	resolver_(io),
	timer_(io),
	reply_(16 * 1024),
	closed_(true),
	contentMaxLength_(sz),
	waitTime_(10)
{
}

void HttpAnt::Run(const std::string& url, HttpAnt::CompletionFunctor f)
{
//	std::cout << url << std::endl;
	content_.clear();
	contentLength_ = 0;
	closed_ = false;

	Url::Locator loc;
	if(!loc.fromString(url)) throw std::runtime_error("Bad url. And btw throw from FromString, not from here.");

	std::ostream r(&request_);
	r << "GET " << loc.formatQuery() << " HTTP/1.0\r\n";
	r << "User-Agent: " << app::fullname << "\r\n";
	r << "Accept: */*\r\n";
	r << "Host: " << loc.host << "\r\n";
	r << "Connection: close" << "\r\n";
	r << "\r\n";

	completer_ = f;

	ba::ip::tcp::resolver::query query(loc.host, loc.protocol());
	resolver_.async_resolve(query, bind(&HttpAnt::OnResolved, shared_from_this(), ba::placeholders::error, ba::placeholders::iterator));
	timer_.expires_from_now(seconds(waitTime_));
	timer_.async_wait(bind(&HttpAnt::Timeout, shared_from_this(), _1));
}

void HttpAnt::Timeout(const bs::error_code& err)
{
	if(closed_) return;

	if(!err) Finish(Error(E_TIMEOUT, "timeout"));
	else if(err != ba::error::operation_aborted) Finish(Error(E_SYSTEM, "timer error"));

	closed_ = true;
}

void HttpAnt::Cancel()
{
	resolver_.cancel();
	if(socket_.is_open()) socket_.close();
	bs::error_code err;
	timer_.cancel(err);
}

void HttpAnt::OnResolved(const bs::error_code& err, ba::ip::tcp::resolver::iterator iep)
{
	if(closed_) return;

	if(!err)
	{
		ba::ip::tcp::endpoint endpoint = *iep;
		socket_.async_connect(endpoint, bind(&HttpAnt::OnConnected, shared_from_this(), ba::placeholders::error, ++iep));
	}
	else if(err != ba::error::operation_aborted)
		Finish(Error(E_RESOLVE, "couldn't resolve host address"));
}

void HttpAnt::OnConnected(const bs::error_code& err, ba::ip::tcp::resolver::iterator iep)
{
	if(closed_) return;

	if(!err)
	{
		ba::async_write(socket_, request_, bind(&HttpAnt::OnWrite, shared_from_this(), _1));
	}
	else if(err != ba::error::operation_aborted)
		Finish(Error(E_CONNECT, "couldn't connect to host"));
}

void HttpAnt::OnWrite(const bs::error_code& err)
{
	if(closed_) return;

	if(!err) ba::async_read_until(socket_, reply_, "\r\n\r\n", bind(&HttpAnt::OnHeadersReceived, shared_from_this(), ba::placeholders::error));
	else if(err != ba::error::operation_aborted) Finish(Error(E_TRANSFER, "socket writing error"));
}

void HttpAnt::HandleReply()
{
	std::string s;
	std::istream r( &reply_ );
	r >> s; 
	r >> s;
	if(s != "200") throw std::runtime_error("bad reply code: " + s);
	while(std::getline(r, s) && s != "\r")
	{
	//	std::cout << s << std::endl;
		if(istarts_with(s, "content-length:"))
			contentLength_ = lexical_cast<unsigned int>(trim_copy(s.substr(s.find(":") + 1)));
	}

	if(contentLength_ > contentMaxLength_) throw std::runtime_error("length > data buffer size");

	while(r.good() && (content_.size() < contentLength_ || contentLength_ == 0))
	{
		const char c = r.get();
		if(r.good()) content_.push_back(c);
	}
}

void HttpAnt::OnContent(const bs::error_code& err)
{
	if(closed_) return;

	if(!err)
	{
		std::istream r( &reply_ );
		while(r.good() && (content_.size() < contentLength_ || contentLength_ == 0))
		{
			const char c = r.get();
			if(r.good()) content_.push_back(c);
		}
		if(content_.size() == contentLength_) Finish(Error(E_NONE));
		else ba::async_read(socket_, reply_, ba::transfer_at_least(1), bind(&HttpAnt::OnContent, shared_from_this(), ba::placeholders::error));
		
	}
	else if(err == ba::error::eof)
		Finish(Error(E_NONE));
	else if(err != ba::error::operation_aborted)
		Finish(Error(E_TRANSFER, "socket error while reading content"));
}

void HttpAnt::Finish(const HttpAnt::Error& e)
{
	closed_ = true;
	reply_.consume(reply_.size());
	bs::error_code err;
	timer_.cancel(err);
	resolver_.cancel();
	if(socket_.is_open()) socket_.close();
	completer_(e);
}

void HttpAnt::OnHeadersReceived(const bs::error_code& err)
{
	if(closed_) return;

	if(!err)
	{
		try
		{
			HandleReply();
		}
		catch (std::exception& e)
		{
			Finish(Error(E_LOGIC, e.what()));
			return;
		}

		ba::async_read(socket_, reply_, ba::transfer_at_least(1), bind(&HttpAnt::OnContent, shared_from_this(), ba::placeholders::error));
	}
	else if(err != ba::error::operation_aborted)
		Finish(Error(E_TRANSFER, "socket error while receiving headers"));
}

