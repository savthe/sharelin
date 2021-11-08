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

#ifndef SEARCH_KEYKEEPER_HPP
#define SEARCH_KEYKEEPER_HPP

#include <map>

#include "defs.hpp"
#include "period.hpp"
#include "searcher_impl.hpp"

namespace search_details {

class KeyKeeper
{
public:
	enum { KEY_TTL = 3600 };

	KeyKeeper();
	void add(const QueriableHub& h) { hubs_[h.endpoint.address] = h; }
	void rest(ip::Address, std::time_t);
	void unproxy(ip::Address);
	void tick();

	bool has(ip::Address addr) const { return hubs_.find(addr) != hubs_.end(); }
	QueriableHub selectFor(SearcherImpl*) const;

	void debug() const;

private:
	typedef std::map <ip::Address, QueriableHub> Hubs;

	Hubs hubs_;
	util::Period periodPurge_;
};

} // namespace search_details

#endif // SEARCH_KEYKEEPER_HPP

