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

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include "connector_impl.hpp"
#include "util.hpp"
#include "connectorsmanager.hpp"
#include "../app.hpp"
#include "../appdefs.hpp"
#include "../verb.hpp"

using boost::iequals;
using boost::trim;

namespace g2 {

ConnectorImpl::ConnectorImpl(ConnectorsManager& connectors):
	connectors_(connectors),
	pSocket_(new ba::ip::tcp::socket(app::io())),
	timer_(app::io()),
	request_(64 * 1024),
	reply_(64 * 1024),
	closed_(false)
{
}

ConnectorImpl::~ConnectorImpl()
{
}

void ConnectorImpl::extractHubs(const std::string& line)
{
	std::stringstream s(line);
	std::string hub;
	while(std::getline(s, hub, ','))
	{
		std::size_t pos = hub.find(' ');
		if(pos == std::string::npos) continue;

		ip::Endpoint endpoint;
		if(endpoint.fromString(hub.substr(0, pos)))
			connectors_.discovered(endpoint, util::now());
		//FIXME timestamp
	}
}

void ConnectorImpl::start(ip::Endpoint e)
{
	info_.endpoint = e;
	verb2::g2 << "Connecting to hub " << e << std::endl;

	info_.connecting = true;

	timer_.expires_from_now(boost::posix_time::seconds(5));
	timer_.async_wait(boost::bind(&ConnectorImpl::onTimeout, shared_from_this(), ba::placeholders::error));

	ba::ip::tcp::endpoint ep(ba::ip::address_v4(info_.endpoint.address.reversed()), info_.endpoint.port);
	pSocket_->async_connect(ep, boost::bind(&ConnectorImpl::onConnected, shared_from_this(), ba::placeholders::error));
}

void ConnectorImpl::onConnected(const bs::error_code& err)
{
	if(!pSocket_->is_open()) return;

	if(!err)
	{
		assert(!closed_);
		info_.connecting = false;

		verb2::g2 << "Connected to " << info_.endpoint << std::endl;

		std::ostream r(&request_);
		r << "GNUTELLA CONNECT/0.6\r\n";

		const ip::Endpoint self = app::endpoint();
		if(!self.empty()) r << "Listen-IP: " << self << "\r\n";

		r << "Remote-IP: " << info_.endpoint.address << "\r\n";
		r << "User-Agent: " << app::fullname << "\r\n";
		r << "Accept: application/x-gnutella2\r\n";
		r << "X-Hub: False\r\n";
		r << "X-Hub-Needed: True\r\n";
		r << "\r\n";

		ba::async_write(*pSocket_, request_, 
				boost::bind(
					&ConnectorImpl::onGreetingSent, shared_from_this(), ba::placeholders::error)
				);
	}
	else if(err != ba::error::operation_aborted)
		onSocketError(err);
}

void ConnectorImpl::onSocketError(const bs::error_code& err)
{
	verb3::g2 << "ConnectorImpl socket error. Endpoint: " << info_.endpoint << " error => " << err.message() << std::endl;
	timer_.cancel();
	detachMe();
}

void ConnectorImpl::onGreetingSent(const bs::error_code& err)
{
	if(!pSocket_->is_open()) return;

	if(!err)
	{
		assert(!closed_);
		assert(request_.size() == 0);
		ba::async_read_until(*pSocket_, reply_, "\r\n\r\n", 
				boost::bind(&ConnectorImpl::onGreetingResponse, shared_from_this(), ba::placeholders::error));
	}
	else if(err != ba::error::operation_aborted)
		onSocketError(err);
}

void ConnectorImpl::onGreetingResponse(const bs::error_code& err)
{
	if(!pSocket_->is_open()) return;
	if(err == ba::error::operation_aborted) return;

	if(err)
	{
		onSocketError(err);
		return;
	}

	assert(!closed_);

	try { handleReply(); }
	catch(std::exception& e)
	{
		verb2::g2 << "Connect to hub " << info_.endpoint << " failed => " << e.what() << std::endl;
		close();
		detachMe();
		return;
	}
}
	
void ConnectorImpl::handleReply()
{
	std::istream r(&reply_);
	std::string line;
	uint code;
	std::string message;

	if(!(r >> line >> code))
		throw std::runtime_error("First line parsing error");
//	verb3::g2 << "Gote reply from host: " << info_.endpoint << " => " << line << " " << code << std::endl;
	if(!iequals(line, "GNUTELLA/0.6"))
		throw std::runtime_error("Not G2 host");
	std::getline(r, message);
//	verb3::g2 << "Headers:" << std::endl;

	while(std::getline(r, line) && line != "\r")
	{
//		verb3::g2 << line << std::endl;
		std::stringstream s(line);
		std::string header;
		std::string value;
		s >> header;
		std::getline(s, value);
		trim(value);

		if(iequals(header, "Remote-IP:"))
		{
			ip::Address addr;
			addr.fromString(value);
			connectors_.onSelf(addr);
		}
		else if(iequals(header, "X-Try-Hubs:"))
			extractHubs(value);
		else if (iequals(header, "User-Agent:"))
			info_.agent = value;
	}
//	verb3::g2 << "End of headers" << std::endl;

	if(code == 200)
	{
		std::ostream r(&request_);
		r << "GNUTELLA/0.6 200 OK\r\n";
		r << "Content-Type: application/x-gnutella2\r\n";
		r << "X-Hub: False\r\n";
		r << "\r\n";
		ba::async_write(*pSocket_, request_, boost::bind(&ConnectorImpl::onG2Connected, shared_from_this(), ba::placeholders::error));
	}
	else throw std::runtime_error(message);
}

void ConnectorImpl::onG2Connected(const bs::error_code& err)
{
	if(!pSocket_->is_open()) return;

	if(!err)
	{
		assert(!closed_);
		closed_ = true; // in case there is timeout scheduled
		timer_.cancel();
		connectors_.onConnected(shared_from_this());
	}
	else if(err != ba::error::operation_aborted)
		onSocketError(err);
}

void ConnectorImpl::onTimeout(const bs::error_code& err)
{
	if(closed_) return;

	if(!err)
	{
		assert(!closed_);
		verb2::g2 << "Connection to hub " << info_.endpoint << " timed out" << std::endl;
		boost::system::error_code err;
	    	pSocket_->shutdown(ba::ip::tcp::socket::shutdown_both, err);	
		pSocket_->close(err);
		closed_ = true;
		detachMe();
	}
	else if(err != ba::error::operation_aborted)
		verb3::g2 << "ConnectorImpl::OnTimer " << info_.endpoint << " error => " << err.message() << std::endl;
}

void ConnectorImpl::close()
{
	if(!closed_)
	{	
		verb3::g2 << "Closing socket " << info_.endpoint << std::endl;
		boost::system::error_code err;
	    	pSocket_->shutdown(ba::ip::tcp::socket::shutdown_both, err);	
		if(pSocket_->is_open()) pSocket_->close(err);
		if(err) verb2::g2 << "There was an error closing socket of ConnectorImpl " << info_.endpoint << std::endl;
		timer_.cancel();
		closed_ = true;
	}
}

void ConnectorImpl::detachMe()
{
	verb3::g2 << "Detaching connector socket " << info_.endpoint << std::endl;
	connectors_.detach(shared_from_this());
}

} // namespace g2
