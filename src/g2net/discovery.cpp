/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov <savthe@gmail.com>

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

#include <vector>
#include <cassert>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

#include "discovery.hpp"
#include "manager.hpp"
#include "url.hpp"
#include "ip.hpp"
#include "../appdefs.hpp"
#include "../settings.hpp"
#include "../app.hpp"
#include "../verb.hpp"
#include "util.hpp"

namespace g2 { 

Discovery::Discovery(Manager& manager):
	manager_(manager),
	pAnt_(new HttpAnt(app::io(), 64 * 1024)),
	period_(boost::posix_time::seconds(1)),
	discover_(false)
{
}

Discovery::~Discovery()
{
}

void Discovery::shutdown()
{
	if(!pAnt_->Closed()) pAnt_->Cancel();
	Save();
}

void Discovery::start()
{
	std::ifstream f(app::discoveryFile().c_str());
	std::string line;
	int goodCount = 0;
	while(std::getline(f, line))
	{
		if(line.empty() || line[0] == '#') continue;
		
		char prefix;
		WebCache gwc;
		std::stringstream s(line);

		if(s >> prefix >> gwc.address)
		{
			if(!(s >> gwc.timestamp) && prefix == 'A') gwc.timestamp = std::time(0);
			else if(prefix == 'B' || prefix == 'X') gwc.banned = true;

			if(!gwc.banned) ++goodCount;

			AddCache(gwc);
		}
	}

	if(goodCount == 0)
	{
		caches_.clear();
		LoadDefaults();
	}
}

bool Discovery::SelectGwc()
{
	address_.clear();

	std::vector<std::string> v;

	for(Caches::iterator i = caches_.begin(); i != caches_.end(); ++i)
	{
		const WebCache& c = i->second;
		if(!c.banned && util::now() - c.timestamp > REST_TIME)
			v.push_back(c.address);
	}

	if(!v.empty()) address_ = v[rand() % v.size()];

	return !address_.empty();
}

void Discovery::tick()
{
	const bool act = discover_ && pAnt_->Closed() && period_.set();	
	if (!act) return;

	discover_ = false;

	if(!SelectGwc())
	{
		LoadDefaults();
		SelectGwc();
	}

	assert(!address_.empty());
	const std::string url = address_ + "?net=gnutella2&support=1&get=1&client=" + std::string(app::vendor) + "&version=" + app::version;
	//std::string url = "http://silvers.zyns.com/gwc/dkac.php?net=gnutella2&support=1&get=1&client=SHLN&version=0.2.2";

	pAnt_->Run(url, bind(&Discovery::OnFinished, this, _1));

	verb1::app << "Discovery started for: " << address_ << std::endl;
}

void Discovery::OnFinished(const HttpAnt::Error& err)
{
	if(err)
	{
		verb1::app << "Couldn't fetch info from GWC ( " << address_ << ") reply => " << err.Message() << std::endl;
		caches_[address_].banned = true;
		return;
	}

	verb1::app << "Parsing GWC reply" << std::endl;

	std::string s;
	pAnt_->CopyContent(std::back_inserter(s));
	try { HandleGwcReply(); }
	catch(std::exception& e)
	{
		verb1::app << "GWC reply => " << e.what() << std::endl;
		caches_[address_].banned = true;
	}
}

void Discovery::HandleGwcReply()
{
	std::stringstream r;
	std::ostream_iterator<char> ir(r);
	pAnt_->CopyContent(ir);

	typedef std::map<ip::Endpoint, uint> Endpoints;
	typedef std::map<std::string, uint> WebCaches;

	Endpoints hubs;
	WebCaches webcaches;

	bool g2support = false;
	uint32 period = 0;
	std::string line;
	while(std::getline(r, line))
	{
		boost::replace_all(line, "|", " ");
		std::stringstream s(line);
		char prefix;
		s >> prefix;
		switch(prefix)
		{
			case 'I': case'i':
			{
				std::string tag;
				std::string val;
				if(s >> tag >> val)
				{
					if(boost::iequals(tag, "support") && boost::iequals(val, "gnutella2"))
						g2support = true;
					else if(boost::iequals(tag, "access") && boost::iequals(val, "period"))
						s >> period;
				}
				break;
			}
			case 'H': case 'h':
			{
				std::string address;
				std::time_t seen = 0;
				if(s >> address)
				{
					s >> seen;
					seen = seen <= util::now() ? seen : util::now();
					ip::Endpoint ep;
					if(ep.fromString(address))
						hubs[ep] = util::since(seen);

				}
				break;
			}
			case 'U': case 'u':
			{
				std::string address;
				uint seen = 0;
				if(s >> address)
				{
					s >> seen;
					webcaches[address] =  util::now() - seen;
				}
			}
		}
	}

	if(g2support)
	{
		verb1::app << "Discovered " << hubs.size() << " hub(s) and " << webcaches.size() << " web cache(s)" << std::endl;
		for(Endpoints::iterator i = hubs.begin(); i != hubs.end(); ++i)
			manager_.touch(i->first, i->second);
		for(WebCaches::iterator i = webcaches.begin(); i != webcaches.end(); ++i)
			AddCache(i->first);
	}
	else
		caches_[address_].banned = true;
}	

void Discovery::LoadDefaults()
{
	verb2::app << "Loading default webcaches" << std::endl;

	caches_.clear();

	AddCache("http://gwc.dyndns.info:28960/gwc.php");
	AddCache("http://karma.cloud.bishopston.net:33559/");
	AddCache("http://cache2.bazookanetworks.com/g2/bazooka.php");
	AddCache("http://cache.trillinux.org/g2/bazooka.php");
	AddCache("http://gwc2.wodi.org/skulls.php");
	AddCache("http://silvers.zyns.com/gwc/dkac.php"); //home, sweet home =)
	AddCache("http://gwc.lame.net/gwcii.php");
	AddCache("http://gwebcache.ns1.net/");
	AddCache("http://cache.wru.pl/skulls.php");
	AddCache("http://jayl.de/gweb/g2/bazooka.php");
	AddCache("http://dogma.cloud.bishopston.net:33559/");
	AddCache("http://midian.jayl.de/g2/bazooka.php");
	AddCache("http://gweb2.4octets.co.uk/gwc.php");
	AddCache("http://cache1.gwebcache.net/skulls.php");
	AddCache("http://cache.wru.pl/skulls.php");
	AddCache("http://cache.trillinux.org/g2/bazooka.php");
	AddCache("http://gwc.marksieklucki.com/skulls.php");
	AddCache("http://karma.cloud.bishopston.net:33559");
	AddCache("http://gwc.agrocampmt.com.br/skulls.php");
	AddCache("http://cache2.gwebcache.net/skulls.php");
	AddCache("http://midian.jayl.de/g2/bazooka.php");
	AddCache("http://cache.ce3c.be/");
	AddCache("http://gwc.lame.net/gwcii.php");
	AddCache("http://cache.leite.us/");
}

void Discovery::AddCache(const std::string& address)
{
	WebCache gwc;
	gwc.address = address;
	//gwc.timestamp = util::now();
	AddCache(gwc);
}

void Discovery::AddCache(const Discovery::WebCache& gwc)
{
	Caches::iterator i = caches_.find(gwc.address);
	if(i != caches_.end() && i->second.banned) 
		return;
	Url::Locator loc;
	if(loc.fromString(gwc.address))
		caches_[gwc.address] = gwc;
}

void Discovery::Save()
{
	std::ofstream f((app::home() + "/discovery.dat").c_str());
	f << "#Web caches. To add a cache use A prefix followed by space and cache address. For example:" << std::endl;
	f << "#A http://some_site/gwc.php" << std::endl;
	f << "#Third parameter is a timestamp. Don't change it manually." << std::endl;
	f << "#Legend: (A)live, (B)anned, (U)ntested." << std::endl;
	const time_t too_old = 60 * 60 * 24 * 7;
	for(Caches::iterator i = caches_.begin(); i != caches_.end(); ++i)
	{
		WebCache gwc = i->second;
		
		if(gwc.banned) f << "B ";
		else if(util::now() - gwc.timestamp  > too_old) f << "U ";
		else f<< "A ";
		
		f << gwc.address << " ";
		f << gwc.timestamp << std::endl;
	}
}

} // namespace g2

