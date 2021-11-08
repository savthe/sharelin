/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010 Andrey Stroganov <savthe@gmail.com>

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

#ifndef INTEGRITY_HPP
#define INTEGRITY_HPP

#include <string>

class Integrity 
{
public:
	typedef unsigned char Color;

	enum {
		NONE = 0,
		VALID = 1, 
		INVALID = 1 << 1, 
		EMPTY = 1 << 2,
		FULL = 1 << 3
	};

	Integrity(): color_(NONE) {}
	Integrity(Color c): color_(c) {}
	Integrity(const Integrity& i): color_(i.color_) {}

	void value(Color c) { color_ = c; }
	Color value() const { return color_; }

	void add(Color c) { color_ |= c; }
	void remove(Color c) { color_ &= ~c; }

	std::string to_string() const;
	bool operator== (const Integrity& rhs) const { return color_ == rhs.color_; }
	bool operator!= (const Integrity& rhs) const { return color_ != rhs.color_; }

private:
//	friend bool operator==(const Integrity&, const Color&);

	Color color_;
};

/*
bool operator==(const Integrity&, const Color&);
*/


#endif // INTEGRITY_HPP


