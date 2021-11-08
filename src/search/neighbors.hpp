/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011 Andrey Stroganov <savthe@gmail.com>

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

#ifndef SEARCH_NEIGHBORS_HPP
#define SEARCH_NEIGHBORS_HPP

#include <set>

#include "ip.hpp"

namespace search_details {

class Neighbors
{
	typedef std::set <ip::Endpoint> Hubs;
public:
	typedef Hubs::const_iterator iterator;

	void add(ip::Endpoint e) { hubs_.insert(e); }
	void remove(ip::Endpoint e) { hubs_.erase(e); }

	iterator begin() 	const { return hubs_.begin(); }
	iterator end() 		const { return hubs_.end(); }
	bool empty() 		const { return hubs_.empty(); }
	std::size_t size()	const { return hubs_.size(); }
	ip::Endpoint random() 	const;
	bool has(ip::Address)	const;

private:
	Hubs hubs_;
};

} // namespace search_details

#endif // SEARCH_NEIGHBORS_HPP

