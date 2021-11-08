/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011  Andrey Stroganov <savthe@gmail.com>

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

#include <boost/bind.hpp>

#include "g2packetreceiver.hpp"

G2PacketReceiver::G2PacketReceiver(TcpSocket& s):
	socket_(s)
{
}

void G2PacketReceiver::receive(Handler h)
{
	handler_ = h;
	packet_.GetBuffer()->clear();
	buffer_.resize(1);
	ba::async_read(socket_, ba::buffer(buffer_), ba::transfer_all(), boost::bind(&G2PacketReceiver::onControlByte, this, ba::placeholders::error));
}

void G2PacketReceiver::appendBuffer()
{
	packet_.GetBuffer()->insert(packet_.GetBuffer()->end(), buffer_.begin(), buffer_.end());
}

void G2PacketReceiver::handle(const bs::error_code& err)
{
	Handler h;
	h.swap(handler_);
	h(packet_, err);
}

void G2PacketReceiver::onControlByte(const bs::error_code& err)
{
	assert(buffer_.size() == 1);

	if(!err)
	{
		appendBuffer();
		const g2::ControlByte cb(buffer_[0]);
		buffer_.resize(cb.lengthSize + cb.nameSize);
		ba::async_read(socket_, ba::buffer(buffer_), ba::transfer_all(), boost::bind(&G2PacketReceiver::onHeader, this, ba::placeholders::error));
	}
	else 
		handle(err);
}

void G2PacketReceiver::onHeader(const bs::error_code& err)
{
	if(!err)
	{
		appendBuffer();

		g2::Header hd;
		try{ hd.Decode(packet_.Bytes(), packet_.Size()); }
		catch( std::exception& e)
		{
			bs::error_code ec(ba::error::eof);
			handler_(packet_, ec);
			return;
		}

		if(hd.dataSize > 0)
		{
			buffer_.resize(hd.dataSize);
			ba::async_read(socket_, ba::buffer(buffer_), ba::transfer_all(), boost::bind(&G2PacketReceiver::onPayload, this, ba::placeholders::error));
			return;
		}
	}

	handle(err);
}

void G2PacketReceiver::onPayload(const bs::error_code& err)
{
	if(!err)
		appendBuffer();

	handle(err);
}

