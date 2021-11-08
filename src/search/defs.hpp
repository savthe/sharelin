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

#ifndef NSSEARCH_DEFS_HPP
#define NSSEARCH_DEFS_HPP

#include <ctime>

#include "types.hpp"
#include "ip.hpp"

namespace search_details {

typedef uint32 QueryKey;

struct QueriableHub
{
	QueriableHub(): qkey(0), keyIssued(0), after(0) {}
	ip::Endpoint endpoint;
	ip::Endpoint proxy;
	QueryKey qkey;
	std::time_t keyIssued;
	std::time_t after;
};

} //namespace search_details

#endif //NSSEARCH_DEFS_HPP

