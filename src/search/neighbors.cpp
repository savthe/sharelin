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

#include <cstdlib>

#include "neighbors.hpp"

namespace search_details {

ip::Endpoint Neighbors::random() const
{
	if (hubs_.empty()) return ip::Endpoint();

	Hubs::const_iterator i = hubs_.begin();
	std::advance(i, std::rand() % hubs_.size()); 

	return *i;
}

bool Neighbors::has(ip::Address addr) const
{
	for (iterator i = begin(); i != end(); ++i)
		if (i->address == addr) return true;

	return false;
}

} // namespace search_details

