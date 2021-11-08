/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009  Andrey Stroganov <savthe@gmail.com>

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

#include <limits>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "searcher_impl.hpp"
#include "../g2packet.hpp"
#include "../app.hpp"
#include "../g2tools.hpp"
#include "../filesystem.hpp"
#include "url.hpp"
#include "util.hpp"

using boost::contains;
using boost::to_lower_copy;

namespace {

bool ValidateName(const std::string& name, const std::string& test)
{
	if(name.empty()) return true;

	std::string::size_type i = 0;
	int good = 0, bad = 0;

	while(i < name.size()) 
	{
		char flag;

		while(i < name.size() && name[i] == ' ') ++i;

		if(i == name.size()) break;

		if(name[i] == '-' || name[i] == '+') 
		{
			flag = name[i];
			if(++i == name.size()) break;
		} 
		else flag = '+';

		int j = i;
		if(name[i] == '"') 
		{
			if(++i == name.size()) break;
			while(i < name.size() && name[i] != '"') ++i;
		}
		else
			while(i < name.size() && name[i] != ' ') ++i;

		const bool found = test.find(name.substr(j, i - j)) != std::string::npos;
		if(found && flag == '+') ++good;
		else if(found || flag == '+') ++bad;

		if(i < name.size() && name[i] == '"') --i; //closing "
	}

	return 2 * good > bad;
}

bool ValidateHit(const SearchHit& hit, const SearchCriteria& criteria)
{
	const file_offset_t min = criteria.minSize;
	const file_offset_t max = criteria.maxSize;

	const std::string dn = Url::decode(hit.dn);
	if(dn.find('\\') != std::string::npos) return false; 
	if(dn.find('/') != std::string::npos) return false; 
	if(dn.find('\0') != std::string::npos) return false; 
	if(dn == "." || dn == "..") return false;

	if(criteria.mediaType)
	{
		if(hit.dn.empty()) return false;
		if(media::resolveByName(hit.dn) != criteria.mediaType) return false;
	}

	if(!criteria.extensions.empty())
	{
		std::string ext = FileSystem::Extension(hit.dn);
		boost::to_lower(ext);
		if(!ext.empty() && ext[0] == '.') ext.erase(0, 1);
		if(criteria.extensions.find(ext) == criteria.extensions.end())
			return false;
	}

	if(hit.sha1.empty() && hit.ttr.empty() && hit.md5.empty() && hit.ed2k.empty()) 
		return false;
				
	if(min && hit.size < min) return false;
	if(max && hit.size > max) return false;
				
	if( !(criteria.sha1.empty() && criteria.ttr.empty() && criteria.md5.empty() && criteria.ed2k.empty()) )
	{
		const bool match = 
		(!criteria.sha1.empty() && criteria.sha1 == hit.sha1) ||
		(!criteria.ttr.empty() && criteria.ttr == hit.ttr) ||
		(!criteria.md5.empty() && criteria.md5 == hit.md5) ||
		(!criteria.ed2k.empty() && criteria.ed2k == hit.ed2k);

		if(!match) return false;;
	}

	if(!ValidateName(to_lower_copy(criteria.dn), to_lower_copy(hit.dn))) 
		return false;

	return true;
}

} //namespace 

namespace search_details {

template <typename T> void InsertHash(g2::Writer& w, const T& hash, const std::string& prefix)
{
	w.Begin(g2::URN);
	w.Write(prefix.c_str(), prefix.size());
	w.Pod(char(0));
	w.Write(hash.bytes(), hash.size());
	w.Close(g2::URN);
}

void TranslateHit(g2::Reader r, SearchHit& hit)
{
	bool hasSZ = false;
	while(r.next() && !hasSZ) hasSZ |= r.type() == g2::SZ;

	r.reset();

	while (r.next())
	{
		switch(r.type())
		{
			case g2::URN:
				g2::extractUrn(r.child(), hit.sha1, hit.ttr, hit.md5, hit.ed2k);
				break;
			case g2::DN:
			{
				g2::Reader rdn(r.child(), g2::Reader::payload);
				if(!hasSZ) hit.size = rdn.pod<uint32>();
				hit.dn.resize(rdn.left());
				rdn.read(&hit.dn[0], hit.dn.size());
				break;
			}
			case g2::SZ:
			{
				g2::Reader rc(r.child(), g2::Reader::payload);
				hit.size = rc.left() == 8 ? rc.pod<int64>() : rc.pod<int32>();
				break;
			}
		}
	}
}

void SearcherImpl::done(ip::Address addr, std::time_t now)
{
	hubs_.searched(addr, now);
}

void SearcherImpl::buildQuery(g2::Packet& q2) const
{
	buildQuery(q2, ip::Endpoint(), 0);
}

void SearcherImpl::attach(const SearchCriteria& c, Handler h)
{
	if(handler_) throw std::logic_error("Searcher is already attached");

	criteria_ = c;
	handler_ = h;
}

void SearcherImpl::detach()
{
	Handler h;
	h.swap(handler_);
}

void SearcherImpl::buildQuery(g2::Packet& q2, ip::Endpoint endpoint, uint32 qkey) const
{
	g2::Writer w(q2);

	w.Begin(g2::Q2);

	if(!endpoint.empty())
	{
		w.Begin(g2::UDP);
		w.Pod(endpoint.address.value);
		w.Pod(endpoint.port);
		w.Pod(qkey);
		w.Close(g2::UDP);
	}

	if(criteria_.minSize || criteria_.maxSize)
	{
		w.Begin(g2::SZR);
		w.Pod(criteria_.minSize);
		w.Pod(criteria_.maxSize == 0 ? std::numeric_limits<uint64>::max() : criteria_.maxSize);
		w.Close(g2::SZR);
	}

	if(!criteria_.dn.empty())
	{
		w.Begin(g2::DN);
		w.Write(&criteria_.dn[0], criteria_.dn.size());
		w.Close(g2::DN);
	}

	if(!criteria_.sha1.empty() && !criteria_.ttr.empty())
		InsertHash(w, hash::BP(criteria_.sha1, criteria_.ttr), "bp");
	else
	{
		if(!criteria_.sha1.empty())
   			InsertHash(w, criteria_.sha1, "sha1");
		if(!criteria_.ttr.empty())
   			InsertHash(w, criteria_.ttr, "ttr");
	}

	if(!criteria_.md5.empty())
		InsertHash(w, criteria_.md5, "md5");

	if(!criteria_.ed2k.empty())
		InsertHash(w, criteria_.ed2k, "ed2k");

	w.Write(guid_.begin(), Guid::length);
	w.Close(g2::Q2);

}

/*
void fun(const g2::Packet& pk)
{
	for(int j = 0; j < 1000; ++j)
	{
		std::cout << j << std::endl;
		for(int i = 0; i < 1000; ++i)
		{
			std::vector<char> pack;
			pk.Pack(pack);
	//		g2::Packet pk2;
	//		pk2.Assign(&pack[0], pack.size());
		}
	}
}
*/

void SearcherImpl::hit(const g2::Packet& qh2, ip::Endpoint from)
{
//	fun(qh2);
	std::vector <SearchHit> hits;
	std::vector <ip::Endpoint> hubs;
	ip::Endpoint senderEndpoint = from;
	Guid senderGuid;
	std::string nick;
	uint rate = 0;
	g2::Reader r(qh2);
	while (r.next())
	{
		switch(r.type())
		{
			case g2::GU:
			{
				g2::Reader rc(r.child(), g2::Reader::payload);
				rc.read(senderGuid.begin(), 16);
				if(guids_.find(senderGuid) != guids_.end()) return;
				if(senderGuid == app::guid()) return;
				break;
			}
			case g2::NA:
			{
				g2::Reader rc(r.child(), g2::Reader::payload);
				senderEndpoint.address = rc.pod<uint32>();
				senderEndpoint.port = rc.pod<uint16>();
				break;
			}
			case g2::H:
			{
				SearchHit hit;
				TranslateHit(r.child(), hit);
				if(ValidateHit(hit, criteria_)) hits.push_back(hit);				
				break;
			}

			case g2::NH:
			{
				g2::Reader rc(r.child(), g2::Reader::payload);
				ip::Endpoint h;
				h.address = rc.pod<uint32>();
				h.port = rc.pod<uint16>();
				hubs.push_back(h);
				break;
			}
			case g2::UPRO:
			{
				g2::Reader rUpro( r.child() );
				while (rUpro.next())
				{
					if(rUpro.type() == g2::NICK)
					{
						g2::Reader rnick(rUpro.child(), g2::Reader::payload);
						nick = std::string(rnick.data(), rnick.left());
					}
				}
				break;
			}
			case g2::HG:
			{
				g2::Reader rHg(r.child());
				while (rHg.next())
				{
					if(rHg.type() == g2::SS)
					{
						g2::Reader rSs(rHg.child(), g2::Reader::payload);
						rSs.advance(3);
						rate = rSs.pod<uint32>();
					}
				}
				break;
			}

		}
	}

	guids_.insert(senderGuid);

	for(std::size_t i = 0; i < hits.size(); i++) 
	{
		SearchResult sr;
		sr.host.rate = rate;
		sr.host.guid = senderGuid;
		sr.host.nick = nick;
		sr.host.endpoint = senderEndpoint;
		sr.host.hubs = hubs;
		sr.hit = hits[i];
		handler_(sr);
	}
}

} //namespace search_details
