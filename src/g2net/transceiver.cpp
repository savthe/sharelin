/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008  Andrey Stroganov

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

#include <cassert>

#include <boost/bind.hpp>

#include "transceiver.hpp"
#include "packethandler.hpp"
#include "manager.hpp"
#include "debug.hpp"
#include "../verb.hpp"
#include "../g2packet.hpp"
#include "../settings.hpp"
#include "connections.hpp"
#include "../trafmanager.hpp"
#include "../traf.hpp"

namespace g2 {

Transceiver::Transceiver(Manager& manager):
	manager_(manager),
	socket_(app::io(), ba::ip::udp::v4()),
	timer_(app::io(), boost::posix_time::seconds(1)),
	sending_(false),
	throttle_(false)
{
}

void Transceiver::Start(uint16 port)
{
	//traffin_.SetShare(app::settings()->net.maxBwIn * 0.05);
	app::traf_in().attach(&traf_in_);
	app::traf_out().attach(&traf_out_);
	timer_.async_wait(boost::bind(&Transceiver::OnTimer, this, ba::placeholders::error));
	if(app::settings().net.firewalled != settings_types::firewalled::YES)
	{
		socket_.bind(ba::ip::udp::endpoint(ba::ip::udp::v4(), port));
		socket_.async_receive_from(ba::buffer(receiveBuffer_, BUFFER_SIZE), sender_, 
			boost::bind(&Transceiver::OnReceived, this, ba::placeholders::error, ba::placeholders::bytes_transferred));
	}
}

void Transceiver::shutdown()
{
	socket_.close();
	timer_.cancel();
}

void Transceiver::OnReceived(const bs::error_code& err, std::size_t bytesReceived)
{
	if(err == ba::error::operation_aborted) return;

	ip::Endpoint endpoint;
	if(err) verb1::g2 << "Transceiver::OnReceived error => " << err.message() << std::endl;
	else
	{
		endpoint = ip::Endpoint(sender_.address().to_v4().to_ulong(), sender_.port());
		endpoint.address.reverse();
	}

	udp::Header header;
	try
	{
		if(bytesReceived < udp::HEADER_SIZE) throw std::runtime_error("Packet is too small");
		if(!header.Unpack(receiveBuffer_, bytesReceived)) throw std::runtime_error("Bad header");
	}
	catch (std::exception& e)
	{
		verb2::g2 << "Error in transceiver ";
		if(!endpoint.empty()) verb2::g2 << "from " << endpoint << " ";
		verb2::g2 << "=> " << e.what() << std::endl;

		StartReceive();

		return;
	}

	if(bytesReceived == udp::HEADER_SIZE || header.totalParts == 0)
		departure_.Acknowlege(header.id, header.partNumber);
	else 
	{
		if(header.acknowledge)
		{
			udp::Header ackHeader = header;
			ackHeader.deflate = false;
			ackHeader.acknowledge =false;
			ackHeader.totalParts = 0;
			departure_.Put(ackHeader, endpoint);
		}

		g2::Packet pk;
		try
		{
			if(arrival_.Put(receiveBuffer_, bytesReceived, endpoint, pk))
				manager_.packetHandler()->handle(pk, endpoint, ip::protocol::UDP);
		}
		catch(std::exception& e) 
		{
			//g2::Reader r(pk);
			verb1::g2 << "Couldn't handle udp packet from " << endpoint << " => " << e.what() << std::endl;
	//		Debug::PrintHex(pk.GetBuffer()->begin(), pk.GetBuffer()->end());
		}
	}

	traf_in_.consume(bytesReceived);
	/*
	if(traffin_.use(ip::Endpoint(), bytesReceived) == 0)
		app::GetTraffIn()->use(ip::Endpoint(), bytesReceived);
		*/

	StartReceive();
}

void Transceiver::StartReceive()
{
	throttle_ = false;
	if(traf_in_.quota())
		socket_.async_receive_from(ba::buffer(receiveBuffer_, BUFFER_SIZE), sender_, 
			boost::bind(&Transceiver::OnReceived, this, ba::placeholders::error, ba::placeholders::bytes_transferred));
	else throttle_ = true;
}

void Transceiver::send(ip::Endpoint ep, const g2::Packet& packet)
{
	departure_.Put(packet, ep);	
}

void Transceiver::OnTimer(const bs::error_code& err)
{
	if(err == ba::error::operation_aborted) return;
	
	if(!sending_) Dispatch();
	if(throttle_) StartReceive();

	arrival_.PurgeOld();

	//traffin_.OnTimer(app::now());

	timer_.expires_from_now(boost::posix_time::seconds(1));
	timer_.async_wait(boost::bind(&Transceiver::OnTimer, this, ba::placeholders::error));
}

void Transceiver::Dispatch()
{
	assert(!sending_);

	ip::Endpoint endpoint;
	const std::size_t size = departure_.Get(sendBuffer_, endpoint);
	if(size > 0)
	{
		sending_ = true;
		socket_.async_send_to(ba::buffer(sendBuffer_, size), ba::ip::udp::endpoint(ba::ip::address_v4(endpoint.address.reversed()), endpoint.port), 
				boost::bind(&Transceiver::OnSent, this, ba::placeholders::error, ba::placeholders::bytes_transferred));
	}
}

void Transceiver::OnSent(const bs::error_code& err, std::size_t bytesSent)
{
	if(err) verb2::g2 << "Problem while data sending in transceiver => " << err.message() << std::endl;

	sending_ = false;
	Dispatch();
}

} // namespace g2
