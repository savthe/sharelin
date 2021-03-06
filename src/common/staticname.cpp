/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009  Andrey Stroganov

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

#include "staticname.hpp"

uint64 StaticName::fromString(const std::string& str)
{
	uint64 x = 0;

       	for(uint i = 0; i < str.size() && i < 8; ++i)
       		((char*)(&x))[i] = unsigned(str[i]);

       	return x;
}
 
std::string StaticName::toString(uint64 nm)
{
	char c;
       	std::string s;
 
	for(int i = 0; ( c = ((char*)(&nm))[i] ) && (i < 8); ++i)
		s += c;

	return s;
}

