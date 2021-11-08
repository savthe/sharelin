/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012  Andrey Stroganov <savthe@gmail.com>

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

#ifndef	BANDWIDTH_HPP
#define BANDWIDTH_HPP

struct IBandwidth
{
	virtual std::size_t quota() const = 0;
	virtual void consume(std::size_t) = 0;
	virtual ~IBandwidth() {}
};

template<typename T> class TBandwidth: public IBandwidth
{
public:
	TBandwidth(T& obj): obj_(obj) {}
	std::size_t quota() const { return obj_.quota(); }
	void consume(std::size_t n) { obj_.consume(n); }

private:
	T& obj_;
};

#endif // BANDWIDTH_HPP

