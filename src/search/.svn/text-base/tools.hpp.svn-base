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

#ifndef SEARCH_TOOLS_HPP
#define SEARCH_TOOLS_HPP

#include <ctime>
#include <functional>
#include <utility>

#include "defs.hpp"

namespace search_details {

struct OldKey: public std::unary_function <const std::pair<ip::Address, search_details::QueriableHub>, bool>
{
	OldKey(std::time_t now, std::time_t maxAge): now_(now), maxAge_(maxAge) {}

	bool operator()(const std::pair<ip::Address, search_details::QueriableHub>& h) const
	{
		return h.second.keyIssued + maxAge_ > now_;
	}

	std::time_t now_;
	std::time_t maxAge_;
};

struct HasProxy: public std::unary_function <const std::pair<ip::Address, search_details::QueriableHub>, bool>
{
	HasProxy(ip::Address proxy): proxy_(proxy) {}

	bool operator()(const std::pair<ip::Address, search_details::QueriableHub>& h) const { return h.second.proxy.address == proxy_; }

	ip::Address proxy_;
};

struct Rested: public std::unary_function <const std::pair<ip::Address, search_details::QueriableHub>, bool>
{
	Rested(std::time_t now): now_(now) {}

	bool operator()(const std::pair<ip::Address, search_details::QueriableHub>& h) const { return h.second.after < now_; }

	std::time_t now_;
};

template <typename C, typename P> void erase_if(C& c, const P& p)
{
	for (typename C::iterator i = c.begin(); i != c.end();)
		if (p(*i)) 
		{
			typename C::iterator next = i;
			++next;
		       	c.erase(i);
			i = next;
		}
		else ++i;
}

} // namespace search_details

#endif // SEARCH_TOOLS_HPP


