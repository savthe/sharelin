/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov <savthe@gmail.com>

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

#ifndef RANGE_HPP
#define RANGE_HPP

#include <stdexcept>

template <class T> class Range
{
public:
	typedef T IndexType;

	Range(): offset_(T()), length_(T()) {}
	Range(const T& off, const T& len): offset_(off), length_(len) {} 
	virtual ~Range() {}

	Range(const Range<T>& r): offset_(r.offset_), length_(r.length_) {}

	void reset() { set(T(), T()); }
	bool empty() const { return length_ == T(); }

	void set(const T& off, const T& len)
	{
		offset_ = off;
		length_ = len;
	}

	void set_bound(const T& first, const T& last)
	{
		if(first > last) throw std::range_error("first > last");

		offset_ = first;
		length_ = last - first + 1;
	}

	template <class Compatible>
	bool intersects(const Range<Compatible>& r) const
	{
		return !(offset() + length() <= r.offset() || offset() >= r.offset() + r.length());
	}

	template <class Compatible>
	bool includes(const Range<Compatible>& r) const
	{
		return first() <= r.first() && last() >= r.last();
	}

	bool includes(const T& v)
	{
		return offset_ <= v && v < offset_ + length_;
	}

	T offset() const { return offset_; }
	T length() const { return length_; }
	T first() const { return offset_; }
	T last() const { return offset_ + length_ - 1; }

	void offset(const T& val) { offset_ = val; }
	void length(const T& val) { length_ = val; }

private:
	T offset_;
	T length_;
};

template <class T1, class T2>
bool operator< (const Range<T1>& r1, const Range<T2>& r2)
{
	return r1.offset() < r2.offset();
}

template <class T1, class T2>
bool operator== (const Range<T1>& r1, const Range<T2>& r2)
{
	return r1.offset() == r2.offset() && r1.length() == r2.length();
}

template <class T1, class T2>
bool operator!= (const Range<T1>& r1, const Range<T2>& r2)
{
	return !(r1.offset() == r2.offset());
}

#endif //RANGE_HPP

