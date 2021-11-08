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

#ifndef SEARCHER_HPP
#define SEARCHER_HPP

#include <stdexcept>

#include <boost/noncopyable.hpp>

#include "search/handler.hpp"
#include "search/search_criteria.hpp"

class SearchMgr;

namespace search_details { class SearcherImpl; }

class Searcher: private boost::noncopyable
{
public:
	Searcher(SearchMgr&);
	~Searcher();

	void search(const SearchCriteria&, search_details::Handler);
	void cancel();

private:
	search_details::SearcherImpl* impl_;
	SearchMgr& manager_;
	bool attached_;
};


#endif //SEARCHER_HPP

