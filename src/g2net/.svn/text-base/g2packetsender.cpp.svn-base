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

#include "g2packetsender.hpp"

G2PacketSender::G2PacketSender(TcpSocket& s):
	socket_(s)
{
}

void G2PacketSender::send(const g2::Packet& pk, Handler h)
{
	if(handler_) throw std::logic_error("Already sending a packet");

	packet_ = pk;
	handler_ = h;

	ba::async_write(socket_, ba::buffer(packet_.Bytes(), packet_.Size()), boost::bind(&G2PacketSender::onSent, this, ba::placeholders::error));
}

void G2PacketSender::onSent(const bs::error_code& err)
{
	Handler h;
	h.swap(handler_);
	h(err);
}


