/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012  Andrey Stroganov <savthe@gmail.com>

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

#include <cassert>

#include "searcher.hpp"
#include "search_mgr.hpp"
#include "search/searcher_impl.hpp"

Searcher::Searcher(SearchMgr& m): 
	impl_(new search_details::SearcherImpl),
	manager_(m),
	attached_(false)
	{}

Searcher::~Searcher()
{
	assert(!attached_);
	delete impl_;
}

void Searcher::search(const SearchCriteria& c, search_details::Handler h)
{
	if(attached_) throw std::logic_error("Search is already attached");

	impl_->attach(c, h);
	manager_.attach(impl_);
	attached_ = true;
}

void Searcher::cancel()
{
	if(attached_)
	{
		impl_->detach();
		manager_.detach(impl_);
		attached_ = false;
	}
}
	
