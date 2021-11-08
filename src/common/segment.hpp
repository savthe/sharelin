/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov

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

#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <stdexcept>

template <class T> class Segment
{
public:
	Segment(): first_(T()), last_(T()) {}
	Segment(const T& first, const T& last): first_(first), last_(last) {}

	template <class Compatible>
	Segment(const Segment<Compatible>& s): first_(s.first()), last_(s.last()) {}

	void Set(const T& first, const T& last)
	{
		if(first > last) throw std::range_error("first > last");
		first_ = first;
		last_ = last;
	}

	void Clear() { set(T(), T()); }

	template <class Compatible>
	bool intersects(const Segment<Compatible>& s) const
	{
		return !(last_ < s.first() || first_ > s.last());
	}

	template <class Compatible>
	bool contains(const Segment<Compatible>& s) const
	{
		return (first_ < s.first() || first_ == s.first()) && (s.last() < last_ || s.last() == last_);
	}

	bool contains(const T& val) const
	{
		return (first_ < val || first_ == val) && (val < last_ || val == last_);
	}

	T first() const { return first_; }
	T last() const { return last_; }
	void first(const T& val) { set(val, last_); }
	void last(const T& val) { set(first_, val); }

private:
	T first_;
	T last_;
};

template <class T1, class T2>
bool operator< (const Segment<T1>& r1, const Segment<T2>& r2)
{
	return r1.first() < r2.first();
}

template <class T1, class T2>
bool operator== (const Segment<T1>& r1, const Segment<T2>& r2)
{
	return r1.first() == r2.first() && r1.last() == r2.last();
}

template <class T1, class T2>
bool operator!= (const Segment<T1>& r1, const Segment<T2>& r2)
{
	return !(r1 == r2);
}

#endif //SEGMENT_HPP

