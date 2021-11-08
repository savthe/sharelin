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

#ifndef G2_HUBCACHE_HPP
#define G2_HUBCACHE_HPP

#include <ctime>
#include <cassert>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/shared_ptr.hpp>

#include "types.hpp"
#include "ip.hpp"
#include "nodeinfo.hpp"
#include "../g2defs.hpp"

namespace g2 {

class HubCache 
{
private:
	struct Wrap
	{
		struct Address {};
		struct Seen {};

		Wrap(const g2::HubInfo& h): address(h.endpoint.address), seen(h.seen), info(h) {}

		ip::Address address;
		std::time_t seen;
		g2::HubInfo info;
	};

	typedef boost::multi_index_container<
		Wrap, boost::multi_index::indexed_by<
			boost::multi_index::ordered_unique<
				boost::multi_index::tag<Wrap::Address>, BOOST_MULTI_INDEX_MEMBER(Wrap, ip::Address, address)>,
			boost::multi_index::ordered_non_unique<
				boost::multi_index::tag<Wrap::Seen>, BOOST_MULTI_INDEX_MEMBER(Wrap, std::time_t, seen), std::less<std::time_t> > >
	> Cache;

public:
	enum { MAX_SIZE = 500 };

	void insert(const g2::HubInfo&);
	void touch(ip::Endpoint, std::time_t);
	void remove(ip::Address);
	bool save(const std::string&) const;
	bool load(const std::string&);
	g2::HubInfo random() const;

private:
	Cache cache_;
};

} // namespace g2
	
#endif //G2_HUBCACHE_HPP
