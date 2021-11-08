/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011 Andrey Stroganov <savthe@gmail.com>

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

#include "keygather.hpp"
#include "neighbors.hpp"
#include "app.hpp"
#include "g2packet.hpp"
#include "verb.hpp"
#include "g2net.hpp"
#include "util.hpp"
#include "tools.hpp"
#include "keykeeper.hpp"

namespace search_details {

KeyGather::KeyGather(KeyKeeper& k):
	keeper_(k),
	periodPurge_(boost::posix_time::seconds(1)),
	needKeys_(20)
{
}

void KeyGather::debug() const
{
}

void KeyGather::onQKA(const g2::Packet& qka, ip::Endpoint endpoint, ip::protocol::Type proto)
{
	//std::cout << "GOT QKA from " << endpoint << " need: " << requestKeys_ << std::endl;
	QueryKey qkey = 0;
	ip::Endpoint proxy;
	ip::Endpoint remote;

	if(proto == ip::protocol::UDP)
	{	
		remote = endpoint;
		proxy = app::endpoint();
	}
	else proxy = endpoint;

	g2::Reader r(qka);
	while (r.next())
	{
		switch(r.type())
		{
			case g2::QK: 
			{
				g2::Reader rc(r.child(), g2::Reader::payload);
				qkey = rc.pod<uint32>(); 
				break;
			}
			case g2::SNA: 
			{
				g2::Reader rc(r.child(), g2::Reader::payload);
				ip::Address addr(rc.pod<uint32>());
				break;
			}
			case g2::QNA: 
			{
				g2::Reader rqna(r.child(), g2::Reader::payload);
				remote.address.value = rqna.pod<uint32>();
				if(rqna.left()) remote.port = rqna.pod<uint16>();
			}
		}
	}

	if(qkey == 0) return;
	
	const IssuedHubs::iterator i = issued_.find(remote.address);
	if (i == issued_.end()) return;

	QueriableHub hub;
	hub = i->second;
	hub.proxy = proxy;
	hub.qkey = qkey;
	hub.keyIssued = util::now();
	keeper_.add(hub);

	verb2::sr << "Got query key " << qkey << " for " << hub.endpoint;
	if(hub.proxy.empty()) verb2::sr << " Issued for self" << std::endl;
	else verb2::sr << " Issued for " << hub.proxy << std::endl;

	if(needKeys_ > 0) --needKeys_;
}

void KeyGather::tick()
{
	if (periodPurge_.set()) 
		erase_if(issued_, OldKey(util::now(), 10));

	if (!needKeys_) return;
	if (!app::g2net().online()) return;
	if (issued_.size() == MAX_ISSUE) return;

	const ip::Endpoint e = app::g2net().randomHub();

	const bool bad = 
		e.empty() || 
		issued_.find(e.address) != issued_.end() ||
		keeper_.has(e.address) || 
		app::g2net().is_connected(e.address);

	if (!bad) query(e);
}

void KeyGather::query(ip::Endpoint e)
{
	g2::Packet qkr;
	g2::Writer w(qkr);

	QueriableHub hub;
	hub.endpoint = e;

	hub.keyIssued = util::now();

	if(app::firewalled())
	{
		hub.proxy = app::g2net().randomNeighborHub();
		if(hub.proxy.empty()) return;

		w.Begin(g2::QKR);
		w.Begin(g2::QNA);
		w.Pod(hub.endpoint.address.value);
		w.Pod(hub.endpoint.port);
		w.Close(g2::QNA);
		w.Close(g2::QKR);

		verb3::sr << "Requesting query key from " << hub.endpoint << 
			" Using " << hub.proxy << " as proxy. Need keys: " << needKeys_ << std::endl;

		app::g2net().sendTcp(hub.proxy.address, qkr);
	}
	else
	{
		const ip::Endpoint self = app::endpoint();
		assert(!self.empty());

		w.Begin(g2::QKR);
		w.Begin(g2::RNA);
		w.Pod(self.address.value);
		w.Pod(self.port);
		w.Close(g2::RNA);
		w.Close(g2::QKR);

		verb2::sr << "Requesting query key from " << hub.endpoint << 
			" Expecting direct reply. Want keys: " << needKeys_ << std::endl;

		app::g2net().sendUdp(hub.endpoint, qkr);
	}

	issued_[hub.endpoint.address] = hub;

//	hub.KeyIssued(app::now());
//	app::GetHubCache()->Update(hub);
}

} // namespace search_details

