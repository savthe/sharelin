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

#ifndef SEARCH_QUEUE_HPP
#define SEARCH_QUEUE_HPP

#include <queue>
#include <ctime>
#include <utility>

#include "searcher_impl.hpp"

namespace search_details {

class Queue
{
public:
	SearcherImpl* pop();
	void push(SearcherImpl* p, std::time_t a) { queue_.push(std::make_pair(p, a)); }
	void remove(SearcherImpl*); 

	bool empty() const { return queue_.empty(); }
	std::time_t after() const;
	std::size_t size() const { return queue_.size(); }
	void clear();

private:
	typedef std::pair <SearcherImpl*, std::time_t> Element;

	struct Comparer
	{
		bool operator()(const Element& e1, const Element& e2) const { return e2.second < e1.second; }
	};

	typedef std::priority_queue <Element, std::vector <Element>, Comparer> Container;
	Container queue_;
};


} // namespace search_details

#endif // SEARCH_QUEUE_HPP

