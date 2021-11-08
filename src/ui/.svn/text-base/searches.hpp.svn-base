/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012 Andrey Stroganov <savthe@gmail.com>

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

#ifndef UI_SEARCHES_HPP
#define UI_SEARCHES_HPP

#include <set>

#include "../../search/search_criteria.hpp"
#include "../../search/user_search.hpp"
#include "idgen.hpp"

class UiSearches
{
	typedef std::set<UserSearch::Ptr> Searches;

public:
	typedef Searches::const_iterator iterator;

	iterator begin() const { return searches_.begin(); }
	iterator end() const { return searches_.end(); }
	Id create(const SearchCriteria&);
	UserSearch::Ptr get(Id) const;
	void remove(UserSearch::Ptr);
	std::size_t count() const { return searches_.size(); }
	void clear();

private:

	Searches searches_;
	Misc::IdGenerator idgen_;
};

#endif // UI_SEARCHES_HPP

