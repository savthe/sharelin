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

#ifndef SHARE_TOOLS_HPP
#define SHARE_TOOLS_HPP

#include <ostream>
#include <istream>

#include "../fileinfo.hpp"

namespace share_details {

std::ostream& operator<<(std::ostream&, const FileInfo&); 
std::ostream& operator<<(std::ostream&, const TigerTree&);
std::ostream& operator<<(std::ostream&, const FileMap&);

std::istream& operator>>(std::istream&, FileInfo&); 
std::istream& operator>>(std::istream&, TigerTree&);
std::istream& operator>>(std::istream&, FileMap&);

template <typename H, int sz, typename C>
std::istream& operator>>(std::istream& is, Hash<H, sz, C>& h)
{
	std::string s;
	is >> s;
	h.fromString(s);

	return is;
}

} // namespace share_details

#endif // SHARE_TOOLS_HPP

