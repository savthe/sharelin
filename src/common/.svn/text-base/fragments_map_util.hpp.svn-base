
/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2010  Andrey Stroganov <savthe@gmail.com>

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

#ifndef FRAGMENTS_MAP_UTIL_HPP
#define FRAGMENTS_MAP_UTIL_HPP

#include <functional>

namespace FragmentsMapUtil
{

template <class F> 
inline typename F::IndexType first(const F& f)
{
	return f.offset();
}

template <class F> 
inline typename F::IndexType last(const F& f)
{
	assert(f.length() > 0);
	return f.offset() + f.length() - 1;
}

template <class F>
struct DefaultComparer: std::binary_function<const F&, const F&, bool>
{
	bool operator()(const F& f1, const F& f2) const  
	{
		return &f1 == &f2;
	}
};

} // namespace FragmentsMapUtil

#endif // FRAGMENTS_MAP_UTIL_HPP

