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

#ifndef TYPE2STREAM_HPP
#define TYPE2STREAM_HPP

#include "range.hpp"

template <typename Elem, typename Traits, typename T>
std::basic_ostream <Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& os, Range<T> r)
{
	os << r.first() << "-" << r.last();
	return os;
}

#endif //TYPE2STREAM_HPP

