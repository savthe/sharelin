/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009-2011  Andrey Stroganov <savthe@gmail.com>

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

#ifndef SEARCH_HOLDER_HPP
#define SEARCH_HOLDER_HPP

#include <map>

#include "searcher_impl.hpp"
#include "guid.hpp"

namespace search_details {

class Holder
{
public:
	void attach(SearcherImpl* p) { searchers_[p->guid()] = p; }
	void detach(SearcherImpl* p) { searchers_.erase(p->guid()); }
	std::size_t size() const { return searchers_.size(); }
	void clear() { searchers_.clear(); }

	SearcherImpl* get(Guid) const;

private:
	typedef std::map<Guid, SearcherImpl*> Searchers;

	Searchers searchers_;
};

} // namespace search_details

#endif // SEARCH_HOLDER_HPP
