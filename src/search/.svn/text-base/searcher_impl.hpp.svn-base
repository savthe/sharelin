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

#pragma once

#include <vector>
#include <set>
#include <ctime>

#include <boost/shared_ptr.hpp>

#include "types.hpp"
#include "ip.hpp"
#include "guid.hpp"
#include "../g2packet.hpp"
#include "defs.hpp"
#include "search_criteria.hpp"
#include "searchedhubs.hpp"
#include "handler.hpp"

namespace search_details {

class SearcherImpl
{
public:
	void hit(const g2::Packet& pk, ip::Endpoint from);
	void done(ip::Address, std::time_t);

	Guid guid()			const { return guid_; }
	void generate_guid() { guid_.generate(); }
	const SearchCriteria& criteria() 	const { return criteria_; }
	bool searched(ip::Address addr) const { return hubs_.searched(addr); }
	std::size_t fame()		const { return hubs_.size(); }
	void attach(const SearchCriteria&, Handler);
	void detach();

	void buildQuery(g2::Packet& packet, ip::Endpoint endpoint, uint32 qkey) const;
	void buildQuery(g2::Packet& packet) const;

private:
	SearchCriteria criteria_;
	Guid guid_;
	SearchedHubs hubs_;
	std::set<Guid> guids_;
	Handler handler_;
};

} //namespace search_details

