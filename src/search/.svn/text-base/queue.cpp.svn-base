/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009-2011 Andrey Stroganov <savthe@gmail.com>

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

#include <limits>

#include "queue.hpp"

namespace search_details {

SearcherImpl* Queue::pop()
{
	SearcherImpl* p(0);

	if (!queue_.empty())
	{
		p = queue_.top().first;
		queue_.pop();
	}

	return p;
}

void Queue::clear()
{
	while(!queue_.empty())
		queue_.pop();
}

void Queue::remove(SearcherImpl* p)
{
	Container tmp;

	while (!queue_.empty())
	{
		if (queue_.top().first != p)
			tmp.push(queue_.top());

		queue_.pop();
	}

	std::swap(queue_, tmp);
}

std::time_t Queue::after() const 
{
	if (queue_.empty()) return std::numeric_limits <std::time_t>::max();

	return queue_.top().second;
}

} // namespace search_details

