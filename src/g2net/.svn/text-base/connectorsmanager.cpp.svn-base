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

#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include "connectorsmanager.hpp"
#include "hubs.hpp"
#include "packethandler.hpp"
#include "manager.hpp"
#include "hubs.hpp"
#include "discovery.hpp"
#include "connector_impl.hpp"
#include "hubcache.hpp"
#include "settings.hpp"
#include "verb.hpp"

using boost::bind;

namespace g2 {

ConnectorsManager::ConnectorsManager(Manager& manager): 
	manager_(manager) 
{
}

void ConnectorsManager::discovered(ip::Endpoint e, std::time_t t)
{
	manager_.touch(e, t);
}

void ConnectorsManager::connect()
{
	const HubInfo hub = manager_.hubCache().random();

	const ip::Endpoint e = hub.endpoint;

	if(e.empty()) 
	{
		manager_.discovery().discover();
		return;
	}

	const bool connected = manager_.hubs().is_connected(e.address) || is_connected(e.address);

	if(!connected)
	{
		Connector::Ptr p(new ConnectorImpl(*this));
		p->start(e);
		nodes_.attach(e.address, p);
	}
}

void ConnectorsManager::onSelf(ip::Address addr)
{
	manager_.onSelf(addr);
}

void ConnectorsManager::detach(Connector::Ptr p)
{
	nodes_.detach(p);
}

void ConnectorsManager::onConnected(Connector::Ptr p)
{
	verb1::g2 << "Connected to G2 hub " << p->info().endpoint << " (" << p->info().agent << ")" << std::endl;

	const ip::Endpoint endpoint = p->info().endpoint;
	const ip::Address address = endpoint.address;
	
	nodes_.detach(p);

	assert(!manager_.hubs().is_connected(address));
	assert(!nodes_.is_connected(address));

	try
	{
		//Node::Ptr p(new NodeImpl(pConnector, *pHubs_));
		//p->start();
		manager_.hubs().attach(p);
	}
	catch (std::exception& e)
	{
		verb1::g2 << "Couldn't connect to hub " << endpoint << " => " << e.what() << std::endl;
	}

//	handshakes_.erase(address);

	if(manager_.hubs().count() == app::settings().net.maxHubs)
		shutdown();
		//pConnectors_->closeAll();
	/*
	{
		for(Handshakes::iterator i = handshakes_.begin(); i != handshakes_.end(); ++i)
			i->second->Close();
		handshakes_.clear();
	}
	*/

}

void ConnectorsManager::tick()
{
	const bool mayConnect = 
		manager_.hubs().count() < app::settings().net.maxHubs && count() < 10;

	if(mayConnect) connect(); 
}

} //namespace g2

