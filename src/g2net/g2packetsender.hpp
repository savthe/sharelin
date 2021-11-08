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

#ifndef G2PACKETSENDER_HPP
#define G2PACKETSENDER_HPP

#include <boost/function.hpp>

#include "io.hpp"
#include "../g2packet.hpp"

class G2PacketSender
{
public:
	typedef boost::function<void (const bs::error_code&)> Handler;

	G2PacketSender(TcpSocket&);

	void send(const g2::Packet&, Handler);
	bool sending() const { return handler_; }

private:
	void onSent(const bs::error_code&);

	TcpSocket& socket_;
	g2::Packet packet_;
	Handler handler_;
};

#endif //G2PACKETSENDER_HPP

