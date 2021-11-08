/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012 Andrey Stroganov <savthe@gmail.com>

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

#include "search.hpp"
#include "holder.hpp"
#include "util.hpp"
#include "neighbors.hpp"
#include "keygather.hpp"
#include "keykeeper.hpp"
#include "g2packet.hpp"
#include "tools.hpp"
#include "app.hpp"
#include "g2net.hpp"
#include "verb.hpp"
#include "debug.hpp"

namespace search_details {

Search::Search(KeyKeeper& k, KeyGather& g):
	keeper_(k),
	gather_(g),
	tick_(boost::posix_time::milliseconds(250))
{
}

void Search::detach_all()
{
	holder_.clear();
	queue_.clear();
}

void Search::detach(SearcherImpl* s) 
{ 
	verb1::sr << "Detaching search" << std::endl;
	holder_.detach(s);
	queue_.remove(s);
}

void Search::debug() const 
{
#ifdef DEBUG
	std::cout << "Holding search_detailss: " << holder_.size() << " queue size: " << queue_.size() << std::endl;

//	queue_.debug();
#endif 
}

void Search::onQA(const g2::Packet& pk, ip::Endpoint endpoint, ip::protocol::Type proto)
{
	g2::Reader r(pk);

	r.seekPayload();
	Guid guid;
	r.read(guid.begin(), 16);

	SearcherImpl* p = holder_.get(guid);
	if(!p) return;

	const std::time_t now = util::now();

	if(proto == ip::protocol::UDP) p->done(endpoint.address, now);

	r.reset();
	while (r.next())
	{
		switch (r.type())
		{
			case g2::D: 
			{
				g2::Reader rc(r.child(), g2::Reader::payload);
				p->done(ip::Address(rc.pod<uint32>()), now);
				break;
			}
			case g2::S: 
			{
				g2::Reader rc(r.child(), g2::Reader::payload);
				ip::Endpoint ep;
				ep.address = rc.pod<uint32>();
				ep.port = rc.pod<uint16>();
				app::g2net().discovered(ep, now); 
				break;
			}
			case g2::FR:
			{
				g2::Reader rfr(r.child(), g2::Reader::payload);
				p->done(ip::Address(rfr.pod<uint32>()), now);
				break;
			}
			case g2::RA:
			{
				//uint tryAfter = r.Child().BytesLeft() == 2 ? r.Child().Pod<uint16>() : r.Child().Pod<uint32>();
				break;
			}
		}
	}
}

void Search::onQH2(const g2::Packet& qh2, ip::Endpoint ep)
{
	Guid guid;
	g2::Reader r(qh2, g2::Reader::payload);
	r.advance(1);
	r.read(guid.begin(), 16);

	SearcherImpl* p = holder_.get(guid);
	if(p) p->hit(qh2, ep);
}

void Search::tick()
{
	const bool hasWork = !queue_.empty() && tick_.set() && util::now() > queue_.after();

	if (!hasWork) return;

	SearcherImpl* p = queue_.pop();
	assert(p);

//	verb3::sr << "Initiating search id:" << p->id() << " fame: " << p->fame() << std::endl;
	std::time_t delay = 0;
	const double scale = 0.3;

	if(localSearch(p) || foreignSearch(p)) 
		delay = std::min(static_cast<std::size_t>(p->fame() * scale), static_cast<std::size_t>(5 * 60));
	else 
	{
		delay = 60;
		gather_.requestKeys();
	}
	//verb3::sr << "Delaying search id: " << p->id() << " for: " << delay << std::endl;

	queue_.push(p, util::now() + delay);
}

bool Search::localSearch(SearcherImpl* p)
{
	ip::Address addr;

	g2::Endpoints neighbors = app::g2net().getNeighbors();

	for (std::size_t i = 0; addr.empty() && i < neighbors.size(); ++i)
	{
		const ip::Address a = neighbors[i].address;

		if (!p->searched(a)) addr = a;
	}

	if (!addr.empty())
	{
		p->done(addr, util::now());
		g2::Packet q2;
		p->buildQuery(q2);
		app::g2net().sendTcp(addr, q2);
	}

	return !addr.empty();
}

bool Search::foreignSearch(SearcherImpl* p)
{
	const QueriableHub hub = keeper_.selectFor(p);
	if(!hub.qkey) return false;

	p->done(hub.endpoint.address, util::now());
	keeper_.rest(hub.endpoint.address, util::now() + HUB_REST_TIME);

	verb2::sr << "Sending search request to " << hub.endpoint << " proxy: " << hub.proxy << " qkey: " << hub.qkey << std::endl;

	g2::Packet q2;
	ip::Endpoint returnEndpoint = hub.proxy.empty() ? app::endpoint() : hub.proxy;
	p->buildQuery(q2, returnEndpoint, hub.qkey);
	app::g2net().sendUdp(hub.endpoint, q2);
	return true;
}

void Search::attach(SearcherImpl* s)
{
	verb2::sr << "Creating new search" << std::endl;

	s->generate_guid();
	holder_.attach(s);
	queue_.push(s, 0);
}

/*
Searcher Search::create(const Criteria& criteria, Initiator t)
{
	verb2::sr << "Creating new search. Initiator: " << (t == AUTO ? "auto" : "user") << std::endl;
	Guid guid;
	guid.generate();

	SearcherPtr p(new PrivateSearcher(criteria, guid, t == USER ? idgen_.get() : 0));
	holder_.attach(p);
	queue_.push(p, 0);

	return p;
}
*/

} // namespace search_details

