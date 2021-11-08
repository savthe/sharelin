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

#include <stdexcept>

#include <boost/bind.hpp>

#include "user_search.hpp"
#include "search_mgr.hpp"

using boost::bind;

UserSearch::UserSearch(SearchMgr& m, Id id, const SearchCriteria& c):
	searcher_(m), id_(id), criteria_(c)
{
}

void UserSearch::search()
{
	searcher_.search(criteria_, bind(&UserSearch::on_result, this, _1));
}

void UserSearch::cancel()
{
	searcher_.cancel();
}

void UserSearch::on_result(SearchResult r)
{
	r.id = results_.size() + 1;
	results_.push_back(r);
}

const SearchResult& UserSearch::result(Id id) const
{
	if(id > results_.size()) throw std::runtime_error("Bad result ID");

	return results_.at(id - 1);
}


