/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011  Andrey Stroganov

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

#ifndef G2NET_PROBE_HPP
#define G2NET_PROBE_HPP

#include <ctime>

#include "ip.hpp"
#include "../g2packet.hpp"

namespace g2 {

class Manager;

class Probe
{
public:
	enum { PROBE_PERIOD = 20 };

	Probe(Manager&);

	void start();
	void tick();
	void onPacketPO(const Packet&, ip::Endpoint, ip::protocol::Type);

	bool firewalled() const { return firewalled_; }

private:
	void probe(ip::Address);

	Manager& manager_;
	bool detect_;
	bool firewalled_;
	std::time_t probed_;
};

} // namespace g2

#endif // G2NET_PROBE_HPP

