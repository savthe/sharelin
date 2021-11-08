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

#include "hubs.hpp"
#include "packethandler.hpp"
#include "manager.hpp"
#include "node_impl.hpp"

#include "../verb.hpp"

using boost::bind;

namespace g2 {

Hubs::Hubs(Manager& manager): 
	manager_(manager) 
{
}

void Hubs::onPacket(const g2::Packet& pk, const ip::Endpoint& e)
{
	assert(nodes_.is_connected(e.address));
	manager_.packetHandler()->handle(pk, e, ip::protocol::TCP);
}

void Hubs::attach(Connector::Ptr p)
{
	Node::Ptr pNode(new NodeImpl(p, *this));
	pNode->start();
	nodes_.attach(pNode->info().endpoint.address, pNode);
}

void Hubs::detach(Node::Ptr p)
{
	ip::Address addr = p->info().endpoint.address;
	verb2::g2 << "Terminating connection to hub " << addr << std::endl;
	nodes_.detach(addr);
}

void Hubs::dispatch(const g2::Packet& packet)
{
	nodes_.apply(bind(&Node::send, _1, packet));
}

ip::Endpoint Hubs::random() const
{
	if(nodes_.empty()) return ip::Endpoint();

	Hubs::const_iterator i = nodes_.begin();
	std::advance(i, rand() % nodes_.count());

	return i->info().endpoint;
}

void Hubs::send(ip::Address addr, const Packet& pk)
{
	nodes_.get(addr)->send(pk);
}

void Hubs::tick()
{
	verb3::g2 << "Ticking hubs" << std::endl;
	nodes_.apply(bind(&Node::tick, _1));
	verb3::g2 << "Hubs ticked" << std::endl;
}

} //namespace g2

