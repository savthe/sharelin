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

#include "util.hpp"

#include <boost/algorithm/string.hpp>

namespace util {

void split2(const std::string& base, const std::string& separator, std::string& left, std::string& right)
{
	const std::string::size_type spos = base.find(separator);
	if(spos == std::string::npos) 
		throw std::range_error("Separator not found");
	left = boost::trim_copy(base.substr(0, spos));
	right = boost::trim_copy(base.substr(spos + 1));
}

} //namespace util
