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

#ifndef G2NET_MGR_HPP
#define G2NET_MGR_HPP
 
#include <memory>
#include <vector>

#include <boost/noncopyable.hpp>

#include "ip.hpp"
#include "g2net/nodeinfo.hpp"
#include "g2net/defs.hpp"
#include "../g2packet.hpp"

namespace g2 { class Manager; }

class G2NetMgr: private boost::noncopyable 
{
public:
	G2NetMgr();
	~G2NetMgr();

	void start();
	void debug();
	void tick();
	void shutdown();
	void sendUdp(ip::Endpoint, const g2::Packet&);
	void sendTcp(ip::Address, const g2::Packet&);
	void discovered(ip::Endpoint, std::time_t);

	ip::Endpoint selfEndpoint() const;
	bool firewalled() const;
	g2::Endpoints getNeighbors() const;
	g2::Endpoints getConnectors() const;
	g2::NodeInfoEx getNeighborInfo(ip::Address) const;
	g2::ConnectorInfo getConnectorInfo(ip::Address) const;
	bool online() const;
	ip::Endpoint randomHub() const;
	ip::Endpoint randomNeighborHub() const;
	bool is_connected(ip::Address) const;

private:
	std::auto_ptr <g2::Manager> pManager_;
};

#endif //G2NET_MGR_HPP
