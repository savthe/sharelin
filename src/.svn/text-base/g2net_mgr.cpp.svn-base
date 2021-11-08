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

#include <boost/foreach.hpp>

#include "g2net_mgr.hpp"
#include "g2net/manager.hpp"
#include "g2net/node.hpp"
#include "g2net/transceiver.hpp"
#include "g2net/hubs.hpp"
#include "g2net/hubcache.hpp"
#include "g2net/probe.hpp"
#include "g2net/connectorsmanager.hpp"

using namespace g2;

G2NetMgr::G2NetMgr():
	pManager_(new g2::Manager)
{
}

G2NetMgr::~G2NetMgr()
{
}

void G2NetMgr::debug()
{
}

void G2NetMgr::start()
{
	pManager_->start();
}

bool G2NetMgr::online() const
{
	return pManager_->hubs().count();
}

ip::Endpoint G2NetMgr::randomHub() const
{
	return pManager_->hubCache().random().endpoint;
}

ip::Endpoint G2NetMgr::randomNeighborHub() const
{
	return pManager_->hubs().random();
}

bool G2NetMgr::is_connected(ip::Address addr) const
{
	return pManager_->hubs().is_connected(addr);
}

void G2NetMgr::sendUdp(ip::Endpoint ep, const g2::Packet& pk)
{
	pManager_->transceiver()->send(ep, pk);
}

void G2NetMgr::sendTcp(ip::Address addr, const g2::Packet& pk)
{
	pManager_->hubs().send(addr, pk);
}

void G2NetMgr::discovered(ip::Endpoint ep, std::time_t t)
{
	pManager_->touch(ep, t);
}

void G2NetMgr::tick()
{
	pManager_->tick();
}

void G2NetMgr::shutdown()
{
	pManager_->shutdown();
}

ip::Endpoint G2NetMgr::selfEndpoint() const
{
	return pManager_->selfEndpoint();
}

bool G2NetMgr::firewalled() const
{
	return pManager_->probe()->firewalled();
}

Endpoints G2NetMgr::getNeighbors() const
{
	Endpoints v;
	BOOST_FOREACH(Node& node, pManager_->hubs())
		v.push_back(node.info().endpoint);
	return v;
}

Endpoints G2NetMgr::getConnectors() const
{
	Endpoints v;
	BOOST_FOREACH(Connector& c, pManager_->connectors())
		v.push_back(c.endpoint());
	return v;
}

NodeInfoEx G2NetMgr::getNeighborInfo(ip::Address addr) const
{
	const Node::Ptr p = pManager_->hubs().get(addr);
	return g2::NodeInfoEx(p->info(), p->stats());
}

ConnectorInfo G2NetMgr::getConnectorInfo(ip::Address addr) const
{
	return pManager_->connectors().get(addr)->info();
}

