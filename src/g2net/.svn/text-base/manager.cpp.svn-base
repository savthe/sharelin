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

#include <iterator>

#include <boost/bind/apply.hpp>
#include <boost/foreach.hpp>

#include "manager.hpp"
#include "discovery.hpp"
#include "node.hpp"
#include "transceiver.hpp"
#include "hubcache.hpp"
#include "debug.hpp"
#include "connector_impl.hpp"
#include "node_impl.hpp"
#include "util.hpp"
#include "packethandler.hpp"
#include "hubs.hpp"
#include "probe.hpp"
#include "connectorsmanager.hpp"
#include "settings.hpp"
#include "../appdefs.hpp"
#include "../verb.hpp"
//#include "../g2packet.hpp"
#include "../app.hpp"
//#include "../g2tools.hpp"
//#include "../gate.hpp"

namespace {

} //namespace

namespace g2 {

Manager::Manager():
	pTransceiver_(new Transceiver(*this)),
	periodNodesTimer_(boost::posix_time::seconds(5)),
	periodConnect_(boost::posix_time::millisec(250)),
	pConnectors_(new ConnectorsManager(*this)),
	pHubCache_(new HubCache),
	pDiscovery_(new Discovery(*this)),
	pHandler_(new PacketHandler(*this)),
	pHubs_(new Hubs(*this)),
	pProbe_(new Probe(*this))
{
}

Manager::~Manager()
{
}

void Manager::debug()
{
}

void Manager::shutdown()
{
	if (!pHubCache_->save(app::hubsFile()))
		verb1::net << "Couldn't save hub cache to " << app::hubsFile() << std::endl;

	pHubs_->shutdown();
	pConnectors_->shutdown();
	pTransceiver_->shutdown();
	pDiscovery_->shutdown();
}

PacketHandler* Manager::packetHandler() const
{
	return pHandler_.get();
}

ConnectorsManager& Manager::connectors() const 
{
	return *pConnectors_.get();
}

Probe* Manager::probe() const
{
	return pProbe_.get();
}

Discovery& Manager::discovery() const
{
	return *pDiscovery_.get();
}

Hubs& Manager::hubs() const
{
	return *pHubs_.get();
}

Transceiver* Manager::transceiver() const
{
	return pTransceiver_.get();
}

HubCache& Manager::hubCache() const
{
	return *pHubCache_.get();
}

void Manager::start()
{
	if (!pHubCache_->load(app::hubsFile()))
		verb1::net << "Couldn't load hub cache from " << app::hubsFile() << std::endl;

	const uint port = app::settings().net.listenPort;
	endpoint_.port = port;

	pTransceiver_->Start(port);
	pDiscovery_->start();
	pProbe_->start();
}

void Manager::touch(ip::Endpoint e, std::time_t t)
{
	pHubCache_->touch(e, t);
}

void Manager::tick()
{
	pDiscovery_->tick();
	pConnectors_->tick();

	if(periodNodesTimer_.set())
		pHubs_->tick();

	pProbe_->tick();
}

/*void Manager::dispatch(const g2::Packet& packet) const
{
	pHubs_->dispatch(packet);
}
*/

void Manager::onSelf(ip::Address addr)
{
	if(endpoint_.empty())
		verb1::g2 << "Discovered self address: " << addr << std::endl;
	endpoint_.address = addr;
}

} // namespace g2
