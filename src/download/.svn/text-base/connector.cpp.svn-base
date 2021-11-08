/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012  Andrey Stroganov <savthe@gmail.com>

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

#include "connector.hpp"
#include "downloader.hpp"
#include "../traf.hpp"
#include "util.hpp"
#include "../trafmanager.hpp"
#include "../verb.hpp"
#include "g2net.hpp"
#include "settings.hpp"
#include "app.hpp"

using util::now;
using util::since;
using boost::bind;

namespace download_details {

Connector::Connector(Fetcher& f, Sources& s):
	fetcher_(f),
	sources_(s),
	ticker_(app::io())
	{}

void Connector::start()
{
	ticker_.tick(boost::posix_time::seconds(3), bind(&Connector::tick, this));
}

void Connector::stop()
{
	BOOST_FOREACH(Downloader::Ptr p, downloaders_)
		p->close();

	downloaders_.clear();
	ticker_.cancel();
}

void Connector::tick()
{
	Downloaders dlrs(downloaders_);

	BOOST_FOREACH(Downloader::Ptr p, dlrs)
	{
		const std::time_t timestamp = p->traf().last_activity();

		if(since(timestamp) > 5)
		{
			p->close();
			downloaders_.erase(p);
		}
	}

	if(fetcher_.need_sources()) 
		make_connections();

	ticker_.tick(boost::posix_time::seconds(3), bind(&Connector::tick, this));
}

void Connector::make_connections()
{
	const int max_connections = 5;
	const std::size_t max_per_second = 10;
	const std::size_t max_queue = 15;

	for(int i = 0; i < max_connections; ++i)
	{
		const bool allowed = 
			downloaders_.size() < max_queue && 
			app::traf_in().moment_connections() < max_per_second &&
			Downloader::instances() < app::settings().net.maxDownloads; 

		if(!allowed) break;

		const ip::Endpoint e = select_endpoint();
		if(e.empty()) break;

		verb2::dl << "Connecting to " << e << std::endl;

		Downloader::Ptr p(new Downloader(e, sources_));
		downloaders_.insert(p);
		p->connect(bind(&Connector::on_connected, shared_from_this(), _1, p));
	}
}

void Connector::update_cache()
{
	for(Sources::iterator i = sources_.begin(); i != sources_.end(); ++i)
		cache_.push_back(i->first);

	std::random_shuffle(cache_.begin(), cache_.end());
}

void Connector::on_connected(const bs::error_code& err, Downloader::Ptr p)
{
	verb2::dl << "Downloader " << p->endpoint() << " on_connect: " << err.message() << std::endl;

	if(downloaders_.find(p) == downloaders_.end())
	{
		if(!err) p->close();
		return;
	}

	downloaders_.erase(p);

	if(err) 
	{
		sources_[p->endpoint()].connect_after = now() + 5 * 60;
		push(p->endpoint());
	}
	else fetcher_.insert(p);
}

void Connector::push(ip::Endpoint ep)
{
	if(app::firewalled()) return;

	const Sources::iterator i = sources_.find(ep);
	if(i == sources_.end()) return;
	const SourceHost& host = i->second;

	if(host.guid.trivial()) return;
	if(host.hubs.empty()) return;

	const ip::Endpoint hub = host.hubs[rand() % host.hubs.size()];

	verb2::dl << "Pushing via " << hub << std::endl;
	ip::Endpoint self = app::endpoint();
	g2::Packet push;
	g2::Writer w(push);

	w.Begin(g2::PUSH);

	w.Begin(g2::TO);
	w.Write(host.guid.begin(), 16);
	w.Close(g2::TO);

	w.Pod(self.address.value);
	w.Pod(self.port);

	w.Close(g2::PUSH);
	/*push.payload.write_pod(self.address.value);
	push.payload.write_pod(self.port);
	g2::PacketIterator i = push.packets.add(G2::PacketType::TO);
	i->payload.write(guid.Get(), guid.Get() + 16);
	*/
	app::g2net().sendUdp(hub, push);
}

ip::Endpoint Connector::select_endpoint()
{
	if(cache_.empty()) update_cache();

	std::set<ip::Endpoint> online;

	BOOST_FOREACH(Downloader::Ptr p, downloaders_)
		online.insert(p->endpoint());

	BOOST_FOREACH(Downloader::Ptr p, fetcher_.downloaders())
		online.insert(p->endpoint());

	const std::time_t now = std::time(0);

	while(!cache_.empty())
	{
		const ip::Endpoint e = cache_.back();
		cache_.pop_back();
		if(online.find(e) != online.end()) continue;

		SourceHost& h = sources_[e];
		if(h.connect_after < now)
			return e;
	}

	return ip::Endpoint();
}

} // namespace download_details

