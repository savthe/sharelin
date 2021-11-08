/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011  Andrey Stroganov <savthe@gmail.com>

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

#ifndef UTIL_HPP
#define UTIL_HPP

#include <ctime>
#include <string>
#include <stdexcept>

namespace util {

inline std::time_t now()
{
	return std::time(0);
}

inline std::time_t since(std::time_t tnow, std::time_t past)
{
	if(past > tnow) 
		throw std::runtime_error("Since() argument points to future");

	return tnow - past;
}

inline std::time_t since(std::time_t p)
{
	return since(now(), p);
}

void split2( const std::string& base, const std::string& separator, std::string& left, std::string& right);

} //namespace util

#endif //UTIL_HPP

