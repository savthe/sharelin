/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov

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

#ifndef ARRIVALCACHE_HPP
#define ARRIVALCACHE_HPP

#include <map>
#include <ctime>

#include "types.hpp"
#include "bytepool.hpp"
#include "ip.hpp"
#include "udp.hpp"
#include "../g2packet.hpp"

class ArrivalCache
{
public:
	bool Put(const char* buf, uint length, ip::Endpoint, g2::Packet& pk);
	void PurgeOld();

private:
	typedef std::vector <ByteVector*> PartsContainer;
	struct Packet
	{
		Packet(ip::Endpoint e, udp::Header h): header(h), parts(h.totalParts), created(time(0)), validParts(0), endpoint(e) {}
		udp::Header header;
		PartsContainer parts;
		std::time_t created;
		uint validParts;
		bool deflate_;
		ip::Endpoint endpoint;
	};

	typedef std::map<udp::PacketID, Packet> Cache;
	typedef std::pair<g2::Packet, ip::Endpoint> AddressedPacket;

	bool OnCompleted(const Packet& pk, g2::Packet& g2pk);
	void ErasePacket(Cache::iterator);


	Cache cache_;
	BytePool pool_;
	ByteVector zbuf_;
};

#endif //ARRIVALCACHE_HPP

