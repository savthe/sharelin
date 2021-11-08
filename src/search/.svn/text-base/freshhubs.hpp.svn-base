/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010 Andrey Stroganov <savthe@gmail.com>

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

#ifndef SEARCH_FRESHHUBS_HPP
#define SEARCH_FRESHHUBS_HPP

#include <cstddef>
#include <list>

#include "ip.hpp"

namespace search_details {

class FreshHubs
{
	typedef std::list <ip::Endpoint> Container;

public:
	typedef Container::const_iterator iterator;

	FreshHubs(std::size_t m): max_(m) {}

	bool push(ip::Endpoint);
	void clear() { container_.clear(); }

	iterator begin() 	const { return container_.begin(); }
	iterator end() 		const { return container_.end(); }
	bool empty() 		const { return container_.empty(); }
	std::size_t size() 	const { return container_.size(); }

private:
	Container container_;
	std::size_t max_;
};


} // namespace search_details

#endif // SEARCH_FRESHHUBS_HPP

