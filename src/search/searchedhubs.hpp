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

#ifndef SEARCH_SEARCHEDHUBS_HPP
#define SEARCH_SEARCHEDHUBS_HPP

#include <map>
#include <ctime>

#include "ip.hpp"

namespace search_details {

class SearchedHubs
{
public:
	void searched(ip::Address addr, std::time_t t) { hubs_[addr] = t; }
	void purge();

	bool searched(ip::Address addr) const { return hubs_.find(addr) != hubs_.end(); }
	std::size_t size() 		const { return hubs_.size(); }

private:
	typedef std::map <ip::Address, std::time_t> Hubs;

	Hubs hubs_;
};

} // namespace search_details

#endif // SEARCH_SEARCHEDHUBS_HPP


