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

#include "searchedhubs.hpp"
#include "util.hpp"

namespace search_details {

void SearchedHubs::purge()
{
	const std::time_t now = util::now();
	const std::time_t rest_time = 60 * 60;

	for(Hubs::iterator i = hubs_.begin(); i != hubs_.end();)
	{
		if(util::since(i->second, now) >= rest_time)
		{
			Hubs::iterator next = i;
			++next;
			hubs_.erase(i);
			i = next;
		}
		else ++i;
	}
}

} // namespace search_details

