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

#include <sstream>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include "app.hpp"
#include "gatekeeper_impl.hpp"
#include "conv/conv.hpp"
#include "guid.hpp"
#include "ip.hpp"
#include "gate_mgr.hpp"
#include "verb.hpp"
#include "gatereactor.hpp"

using boost::iequals;
using boost::istarts_with;

GatekeeperImpl::GatekeeperImpl(GateMgr* pGate):
	pGate_(pGate),
	pSock_(new TcpSocket(app::io())),
	timer_(app::io()),
	buffer_(BUFFER_SIZE),
	active_(false)
{
	message_.reserve(BUFFER_SIZE);
}

GatekeeperImpl::~GatekeeperImpl()
{
}

void GatekeeperImpl::start()
{
	timer_.expires_from_now(boost::posix_time::seconds(5));
	timer_.async_wait(boost::bind(&GatekeeperImpl::onTimeout, shared_from_this(), ba::placeholders::error));

	ba::ip::tcp::endpoint ep = pSock_->remote_endpoint();
	endpoint_.address.value = ep.address().to_v4().to_ulong();
	endpoint_.address.reverse();
	endpoint_.port = ep.port();

	read();

	active_ = true;

	verb2::net << "Accepted incomming connection from " << endpoint_ << std::endl;
}

void GatekeeperImpl::push(ip::Endpoint endpoint)
{
	timer_.expires_from_now(boost::posix_time::seconds(5));
	timer_.async_wait( boost::bind ( &GatekeeperImpl::onTimeout, shared_from_this(), ba::placeholders::error));

	endpoint_ = endpoint;
	ba::ip::tcp::endpoint asioEndpoint(ba::ip::address_v4(endpoint.address.reversed()), endpoint.port);

	pSock_->async_connect(asioEndpoint, boost::bind( &GatekeeperImpl::onConnected, shared_from_this(), ba::placeholders::error));

	active_ = true;

	verb2::net << "Connecting by push request to " << endpoint_ << std::endl;
}

void GatekeeperImpl::onConnected(const bs::error_code& err)
{
	if( !active_ ) return ;
//	if(!pSock_->is_open()) return;

	if(!err) 
	{
		verb1::net << "Connected by push to " << endpoint_ << std::endl;
		std::ostream r(&buffer_);
		r << "GIV 0:";
		const Guid guid = app::guid();
		conv::Hex::encode(guid.begin(), guid.end(), std::ostream_iterator<char>(r));
		r << "/\r\n\r\n";
		ba::async_write(*pSock_, buffer_, boost::bind( &GatekeeperImpl::onGreetingSent, shared_from_this(), _1));
	}
	else if(err != ba::error::operation_aborted)
		onSocketError(err);
}

void GatekeeperImpl::onGreetingSent(const bs::error_code& err)
{
	if( !active_ ) return;
//	if(!pSock_->is_open()) return;

	if(!err) read();
	else if(err != ba::error::operation_aborted)
		onSocketError(err);
}

void GatekeeperImpl::close()
{
	active_ = false;
	pSock_->close();
	timer_.cancel(); 
}

void GatekeeperImpl::onTimeout(const bs::error_code& err)
{
	if( !active_ ) return;
	//if (err != ba::error::operation_aborted && pSock_->is_open())
	active_ = false;
	verb2::net << "Gatekeeper " << endpoint_ << " timed out" << std::endl;
	pSock_->close(); 
	pGate_->detach(shared_from_this());
}

void GatekeeperImpl::onSocketError(const bs::error_code& err)
{
	verb3::net << "GatekeeperImpl socket error " << endpoint_ << " error => " << err.message() << std::endl;
	active_ = false;
	timer_.cancel();
	pGate_->detach(shared_from_this());
}

void GatekeeperImpl::read()
{
	ba::async_read(*pSock_, buffer_, ba::transfer_at_least(1), boost::bind( &GatekeeperImpl::onData, shared_from_this(), _1, _2));
}

void GatekeeperImpl::onData(const bs::error_code& err, std::size_t length)
{
	if( !active_ ) return;
//	if(!pSock_->is_open()) return;

	if(!err) 
	{
		std::istream r(&buffer_);
		while(r.good())
		{
			const char c = r.get();
			if(r.good()) message_.push_back(c);
		}

		switch (GateReactor::instance().react(shared_from_this()))
		{
			case GateReactor::ACCEPTED:
				active_ = false;
				timer_.cancel();
				pGate_->detach(shared_from_this());
				break;
			case GateReactor::REJECTED:
				verb2::net << "Can't handle incomming request from " << endpoint_ 
					<< ". Handler not found for " << message_ << std::endl;
				active_ = false;
				timer_.cancel();
				pSock_->close();
				pGate_->detach(shared_from_this());
				break;
			case GateReactor::INCOMPLETE:
				read();
				break;
		}
	} 
	else if(err != ba::error::operation_aborted)
		onSocketError(err);
}

