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

#include <sstream>

#include <boost/algorithm/string.hpp>

#include "tools.hpp"
#include <iostream>

using boost::iequals;

namespace {

std::string TranslateTags(const std::string& tags)
{
	std::string ret(1, char(27));
//	std::string ret;
	ret += "[";
	std::stringstream s(tags);
	std::string t;

	while(s >> t)
	{
		if(iequals(t, "bold")) ret += "1";
		else if(iequals(t, "italic")) ret += "3"; 
		else if(iequals(t, "underline")) ret += "4"; 
		else if(iequals(t, "black")) ret += "30"; 
		else if(iequals(t, "red")) ret += "31"; 
		else if(iequals(t, "green")) ret += "32"; 
		else if(iequals(t, "yellow")) ret += "33"; 
		else if(iequals(t, "blue")) ret += "34"; 
		else if(iequals(t, "magenta")) ret += "35"; 
		else if(iequals(t, "cyan")) ret += "36"; 
		else if(iequals(t, "white")) ret += "37"; 
		else if(iequals(t, "_black")) ret += "40"; 
		else if(iequals(t, "_red")) ret += "41"; 
		else if(iequals(t, "_green")) ret += "42"; 
		else if(iequals(t, "_yellow")) ret += "43"; 
		else if(iequals(t, "_blue")) ret += "44"; 
		else if(iequals(t, "_magenta")) ret += "45"; 
		else if(iequals(t, "_cyan")) ret += "46"; 
		else if(iequals(t, "_white")) ret += "47"; 

		ret += ";";
	}

	if(ret.size() == 1) ret += "0";

	if(*ret.rbegin() == ';') ret.erase(ret.size() - 1, 1);

	ret += "m";

	return ret;
}

} //namespace 

namespace ui {
namespace telnet { 

void FormatAscii(std::string& s)
{
	for(std::string::size_type i = 0; i < s.size(); ++i)
	{
		if(s[i] != '^') continue;

		if(i < s.size() - 1 && s[i + 1] == '^')
		{
			s.erase(i, 1);
			continue;
		}

		if(i == s.size() - 1) continue;
		if(s[i + 1] != '[') continue;

		std::string::size_type k = i + 2;
		std::string tags;
		while(k < s.size() && s[k] != ']') tags += s[k++];

		s.replace(i, k - i + 1, TranslateTags(tags));
	}
}

} // namespace telnet 
} // namespace ui


