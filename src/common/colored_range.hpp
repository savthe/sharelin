/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012 Andrey Stroganov <savthe@gmail.com>

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

#ifndef COLOREDRANGE_HPP
#define COLOREDRANGE_HPP

#include <functional>

#include "range.hpp"

template <class I, class C>
class ColoredRange: public Range<I>
{
public:
	template<class, class> friend struct ColoredRangeComparer;

	ColoredRange(): color_(C()) {}
	ColoredRange(const Range<I>& r, const C c): Range<I>(r), color_(c) {}

	virtual ~ColoredRange() {}

	inline C color() const { return color_; }
	inline void color(const C& c) { color_ = c; }

private:
	C color_;
};

template<class I, class C>
struct ColoredRangeComparer: public std::binary_function<const ColoredRange<I, C>&, const ColoredRange<I, C>&, bool>
{
	bool operator()(const ColoredRange<I, C>& r1, const ColoredRange<I, C>& r2) const 
	{
		return r1.color_ == r2.color_;
	}
};

#endif // COLOREDRANGE


