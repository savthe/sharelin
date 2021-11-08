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

#include <boost/foreach.hpp>

#include "searches.hpp"
#include "../../searches.hpp"

Id UiSearches::create(const SearchCriteria& c)
{
	UserSearch::Ptr p(new UserSearch(app::searches(), idgen_.get(), c));
	p->search();
	searches_.insert(p);

	return p->id();
}

void UiSearches::clear()
{
	BOOST_FOREACH(UserSearch::Ptr p, searches_)
		p->cancel();

	searches_.clear();
}

UserSearch::Ptr UiSearches::get(Id id) const
{
	BOOST_FOREACH(UserSearch::Ptr p, searches_)
		if(p->id() == id)
			return p;

	throw std::runtime_error("Bad search ID");
}

void UiSearches::remove(UserSearch::Ptr p)
{
	p->cancel();
	searches_.erase(p);
}
