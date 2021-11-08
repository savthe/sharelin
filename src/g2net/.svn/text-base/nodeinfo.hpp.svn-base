/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010 Andrey Stroganov <savthe@gmail.com>

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

#ifndef G2NET_NODEINFO_HPP
#define G2NET_NODEINFO_HPP

#include <ctime>

#include "ip.hpp"
#include "guid.hpp"
#include "types.hpp"
#include "../g2defs.hpp"

namespace g2 {

struct NodeInfo
{
	ip::Endpoint endpoint;
	Guid guid;
	uint32 vendor;
	std::string agent;
};

struct HubInfo 
{
	HubInfo(): seen(0) {}
	HubInfo(ip::Endpoint ep): seen(0) {}
	HubInfo(ip::Endpoint ep, std::time_t t): endpoint(ep), seen(t) {}

	ip::Endpoint endpoint;
	std::time_t seen;
};


struct NodeStats
{
	NodeStats(): goodPackets(0), badPackets(0), sentPackets(0), startTime(0) {}
	uint goodPackets;
	uint badPackets;
	uint sentPackets;
	std::time_t startTime;
};

struct NodeInfoEx
{
	NodeInfoEx() {}
	NodeInfoEx(const NodeInfo& i, const NodeStats& s): info(i), stats(s) {}
	NodeInfo info;
	NodeStats stats;
};

struct ConnectorInfo
{
	ConnectorInfo(): connecting(false) {}
	ip::Endpoint endpoint;
	std::string agent;
	bool connecting;
};


} //namespace G2

bool operator< (const g2::HubInfo& hub1, const g2::HubInfo& hub2);
bool operator== (const g2::HubInfo& hub1, const g2::HubInfo& hub2);
bool operator!= (const g2::HubInfo& hub1, const g2::HubInfo& hub2);

#endif //G2NET_NODEINFO_HPP
