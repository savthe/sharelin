/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2009  Sharelin Project

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

#include <fstream>
#include <sstream>
#include <ctime>
#include <cassert>

#include "debug.hpp"
#include "hubcache.hpp"

namespace g2 {

bool HubCache::save(const std::string& fname) const
{
	std::ofstream f(fname.c_str());
	
	if (!f.good()) return false;

	f << "#Sharelin hub cache." << std::endl;
	f << "#To add hub manually just type it's address and port. Don't add timestamp." << std::endl;

	for (Cache::iterator i = cache_.begin(); i != cache_.end(); ++i)
		f << i->info.endpoint << " " << i->info.seen << "\n";

	f.close();

	return true;
}

bool HubCache::load(const std::string& fname)
{
	std::ifstream f(fname.c_str());

	if(!f.good()) return false;

	std::string line;
	while(std::getline(f, line))
	{
		if(line.empty() || line[0] == '#')
			continue;

		std::stringstream s(line);
		std::string strEndpoint;
		uint32 ts = 0;
		s >> strEndpoint >> ts;	
		ip::Endpoint ep;
		if(ep.fromString(strEndpoint)) 
			touch(ep, ts);
	}

	f.close();

	return true;
}

void HubCache::insert(const g2::HubInfo& h)
{
	if(h.endpoint.empty()) return;

	Cache::index_iterator<Wrap::Address>::type i = cache_.get<Wrap::Address>().find(h.endpoint.address);

	if (i == cache_.get<Wrap::Address>().end())
		cache_.insert(Wrap(h));
	else
		cache_.replace(i, Wrap(h));

	if(cache_.size() > MAX_SIZE)
		cache_.get<Wrap::Seen>().erase(cache_.get<Wrap::Seen>().begin());

}

void HubCache::touch(ip::Endpoint endpoint, std::time_t ts)
{
	if(endpoint.empty()) return;

	Cache::index_iterator<Wrap::Address>::type i = cache_.get<Wrap::Address>().find(endpoint.address);
	if(i == cache_.get<Wrap::Address>().end())
		insert(g2::HubInfo(endpoint, ts));
	else
	{
		g2::HubInfo info = i->info;
		info.seen = ts;
		cache_.replace(i, Wrap(info));
	}
}

void HubCache::remove(ip::Address addr)
{
	cache_.get<Wrap::Address>().erase(addr);
}

/*
g2::HubInfo HubCache::Find(ip::Address addr) const
{
	g2::HubInfo hub;

	Cache::index_iterator<tagAddress>::type i = cache_.get<tagAddress>().find(addr);
	if(i != cache_.get<tagAddress>().end()) hub = i->info;

	return hub;
}
*/

g2::HubInfo HubCache::random() const
{
	if(cache_.empty()) return g2::HubInfo();

	Cache::iterator i = cache_.begin();
	std::advance(i, rand() % cache_.size());

	return i->info;
}

} // namespace g2

