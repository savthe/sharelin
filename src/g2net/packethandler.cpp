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
#include <search/search_criteria.hpp>

#include "packethandler.hpp"
#include "manager.hpp"
#include "hubcache.hpp"
#include "util.hpp"
#include "hubs.hpp"
#include "transceiver.hpp"
#include "probe.hpp"
#include "verb.hpp"
#include "gate.hpp"
#include "searches.hpp"
#include "share.hpp"
#include "settings.hpp"
#include "app.hpp"
#include "g2tools.hpp"

namespace g2 {

PacketHandler::PacketHandler(Manager& manager):
	manager_(manager)
{
}

void PacketHandler::handle(const g2::Packet& pk, ip::Endpoint ep, ip::protocol::Type proto)
{
	g2::Reader r(pk);

	switch (r.type())
	{
		case g2::PO: manager_.probe()->onPacketPO(pk, ep, proto); break;
		case g2::PUSH: onPacketPUSH(pk, ep, proto); break;
		case g2::Q2: onPacketQ2(pk, ep, proto); break;
		case g2::KHL: onPacketKHL(pk, ep, proto); break;
		case g2::QKA: app::searches().onPacketQKA(pk, ep, proto); break;
		case g2::QH2: app::searches().onPacketQH2(pk, ep, proto); break;
		case g2::QA: app::searches().onPacketQA(pk, ep, proto); break;

		/*
		default: 
			verb1::g2 << "Unhandled packet " << StaticName::toString(r.Type()) << " from node " 
			 << endpoint << " via " << (proto == ip::protocol::UDP ? "UDP" : "TCP") << std::endl;
			 */
	}

	if(proto == ip::protocol::TCP)
		manager_.touch(ep, util::now());

}

void PacketHandler::onPacketPUSH(const g2::Packet& pk, ip::Endpoint ep, ip::protocol::Type proto)
{
	verb2::g2 << "Got push from " << ep << std::endl;
	g2::Reader r(pk);
	Guid guid;
	while (r.next())
	{
		if(r.type() == g2::TO)
		{
			g2::Reader rc(r.child(), g2::Reader::payload);
			rc.read(guid.begin(), 16);
		}
	}

	if(guid != app::guid())
	{
		verb2::g2 << "Pushed wrong guid from " << ep << std::endl;
		return;
	}

	ip::Endpoint to;
	to.address = r.pod<uint32>();
	to.port = r.pod<uint16>();

	app::gate().push(to);
}

void PacketHandler::onPacketKHL(const g2::Packet& pk, ip::Endpoint ep, ip::protocol::Type proto)
{
	g2::Reader r(pk);

	const std::time_t now = util::now();
	std::time_t senderTime = now;

	while (r.next())
		if (r.type() == g2::TS) 
		{
			g2::Reader rc(r.child(), g2::Reader::payload);
			senderTime = rc.pod<int32>();
			break;
		}

	r.reset();

	while (r.next())
	{
		if (r.type() == g2::NH)
		{
			g2::Reader rc(r.child(), g2::Reader::payload);

			ip::Endpoint e;
			e.address = rc.pod<uint32>();
			e.port = rc.pod<uint16>();
			manager_.touch(e, now);
		}
		if (r.type() == g2::CH)
		{
			g2::Reader rc(r.child(), g2::Reader::payload);

			ip::Endpoint e;
			e.address = rc.pod<uint32>();
			e.port = rc.pod<uint16>();
			std::time_t ts = rc.pod<int32>();

			if (ts > senderTime) ts = senderTime - 10 * 60;

			if (senderTime > now) ts -= senderTime - now;
			else ts += now - senderTime;

			manager_.touch(e, ts);
		}
	}
}

void PacketHandler::onPacketQ2(const g2::Packet& pk, ip::Endpoint ep, ip::protocol::Type proto)
{
	SearchCriteria criteria;
	ip::Endpoint returnEndpoint;
	Guid guid;
	std::string dn;

	g2::Reader r(pk);
	bool searchUrn = false;

	while (r.next())
	{
		switch ( r.type() )
		{
			case g2::DN: 
			{
				g2::Reader rdn(r.child(), g2::Reader::payload);
				dn.resize(rdn.left());
				rdn.read(&dn[0], dn.size());

				break;
			}
			case g2::UDP: 
			{
				g2::Reader rudp(r.child(), g2::Reader::payload);
				returnEndpoint.address = rudp.pod<uint32>();
				returnEndpoint.port = rudp.pod<uint16>(); 
				break;
			}
			case g2::URN:
			{
				searchUrn = true;
				g2::extractUrn(r.child(), criteria.sha1, criteria.ttr, criteria.md5, criteria.ed2k);
				break;
			}
		}
	}

	r.read(guid.begin(), 16);

	std::vector<SharedFile> results;

	if( searchUrn )
	{
		//std::cout << "." << std::flush;
		SharedFile f = app::share().findHashes(criteria.sha1, criteria.ttr, criteria.ed2k, criteria.md5); 
		if (f)
		{ 
			f->increase_hits();
			results.push_back(f);
			verb2::g2 << "Successful hit: " << f->title() << " ";
			verb2::g2 << "from " << ep << " return: " << returnEndpoint << std::endl;
		}
	}
	else if(!dn.empty())
		app::share().findDn(dn, results, 30);

	if( results.empty() ) return;
	
	g2::Packet qh;
	g2::Writer w(qh);

	w.Begin( g2::QH2 );
		w.Begin( g2::NA );
			w.Pod(manager_.selfEndpoint().address.value);
			w.Pod(manager_.selfEndpoint().port);
			//std::cout << endpoint << std::endl;
		w.Close( g2::NA );

		w.Begin( g2::GU );
			w.Write( app::guid().begin(), 16 );
		w.Close( g2::GU );

		//Endpoints hubs = app::g2net()->getNeighbors();
//		app::g2net()->CopyHubs(std::back_inserter(hubs));
		BOOST_FOREACH (const Node& node, manager_.hubs())
		{
			const ip::Endpoint e = node.info().endpoint;
			w.Begin(g2::NH);
				w.Pod(e.address.value);
				w.Pod(e.port);
			w.Close(g2::NH);
		}
		/*
		std::vector<G2Node::Ptr> neighbors;
		manager_.neighbors()->dump(std::back_inserter(neighbors));
		BOOST_FOREACH (const G2Node::Ptr p, neighbors)
		{
			const ip::Endpoint e = p->info().endpoint;
			w.Begin(g2::NH);
				w.Pod(e.address.value);
				w.Pod(e.port);
			w.Close(g2::NH);
		}
		*/

		w.Begin( g2::UPRO );
			w.Begin( g2::NICK );
				w.Write( app::settings().general.userName );
			w.Close( g2::NICK );
		w.Close( g2::UPRO );	

		for(uint i = 0; i < results.size(); ++i)
		{
			const FileInfo& inf = results[i]->info();

			w.Begin( g2::H );
			if( !inf.sha1.empty() && !inf.ttr.empty() )
			{
				hash::BP bp( inf.sha1, inf.ttr );
				g2::addUrn( w, "bp", bp.bytes(), hash::BP::size() );
			}
			else if( !inf.sha1.empty() )
				g2::addUrn( w, "sha1", inf.sha1.bytes(), hash::SHA1::size() );
			else if( !inf.ttr.empty() )
				g2::addUrn( w, "ttr", inf.ttr.bytes(), hash::TTR::size() );

			if( !inf.ed2k.empty() )
				g2::addUrn( w, "ed2k", inf.ed2k.bytes(), hash::ED2K::size() );
			if( !inf.md5.empty() )
				g2::addUrn( w, "md5", inf.md5.bytes(), hash::MD5::size() );

			w.Begin( g2::URL );
			w.Close( g2::URL );

			w.Begin( g2::SZ );
				w.Pod( inf.size );
			w.Close( g2::SZ );

			w.Begin( g2::DN );
				w.Write( inf.title() );
			w.Close( g2::DN );

			w.Close( g2::H );
		}
				
		w.Pod( char(0) );
		w.Write( guid.begin(), 16 );
	w.Close( g2::QH2 );

	if( returnEndpoint.empty() ) manager_.hubs().send(ep.address, qh);
	else manager_.transceiver()->send(returnEndpoint, qh);
}

} // namespace g2

