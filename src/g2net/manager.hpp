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

#ifndef G2NET_MANAGER_HPP
#define G2NET_MANAGER_HPP

#include <stdexcept>
#include <ctime>
#include <memory>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include "types.hpp"
#include "ip.hpp"
#include "period.hpp"
#include "connector.hpp"
#include "node.hpp"
#include "../gatekeeper.hpp"
#include "../g2packet.hpp"
#include "io.hpp"

namespace g2 {

class Hubs;
class Transceiver;
class Discovery;
class HubCache;
class PacketHandler;
class Probe;
class ConnectorsManager;

class Manager
{
public:
	Manager();
	~Manager();

	void start();
	void tick();
	void shutdown();
	ip::Endpoint selfEndpoint() const { return endpoint_; }
	void onSelf(ip::Address);
	void touch(ip::Endpoint, std::time_t);
	void debug();

	ConnectorsManager& connectors() const;
	Hubs& hubs() const;
	Transceiver* transceiver() const;
	PacketHandler* packetHandler() const;
	HubCache& hubCache() const;
	Probe* probe() const;
	Discovery& discovery() const;

private:

	void OnAccept(Gatekeeper::Ptr, const bs::error_code& error);

	//Handshakes handshakes_;
	std::auto_ptr<Transceiver> pTransceiver_;
	ip::Endpoint endpoint_;
	util::Period periodNodesTimer_;
	util::Period periodConnect_;
	std::auto_ptr <ConnectorsManager> pConnectors_;
	std::auto_ptr <HubCache> pHubCache_;
	std::auto_ptr <Discovery> pDiscovery_;
	std::auto_ptr <PacketHandler> pHandler_;
	std::auto_ptr <Hubs> pHubs_;
	std::auto_ptr <Probe> pProbe_;

};

} // namespace g2

#endif // G2NET_MANAGER_HPP

