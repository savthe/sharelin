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

#ifndef USER_SEARCH_HPP
#define USER_SEARCH_HPP

#include <vector>

#include <boost/shared_ptr.hpp>

#include "types.hpp"
#include "../searcher.hpp"
#include "../search_result.hpp"

class UserSearch
{
public:
	typedef boost::shared_ptr<UserSearch> Ptr;
	typedef std::vector<SearchResult> Results;

	UserSearch(SearchMgr&, Id, const SearchCriteria&);
	void search();
	void cancel();
	Id id() const { return id_; }
	const Results& results() const { return results_; }
	const SearchResult& result(Id id) const;
	const SearchCriteria& criteria() const { return criteria_; }

private:
	void on_result(SearchResult);

	Searcher searcher_;
	Id id_;
	SearchCriteria criteria_;
	std::vector<SearchResult> results_;
};

#endif // USER_SEARCH_HPP

