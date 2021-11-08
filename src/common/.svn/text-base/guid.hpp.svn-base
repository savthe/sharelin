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

#ifndef GUID_HPP
#define GUID_HPP

#include <cstddef>
#include <algorithm>

class Guid
{
public:
	enum { length = 16 };

	typedef char* iterator;
	typedef const char* const_iterator;

	Guid();
	Guid( const Guid& );
	Guid( const char* );

	Guid& operator=( const Guid& );

	void generate();
	void reset();

	bool trivial() const;

	//char* get_buffer() 		{ return bytes_; }
	iterator begin() 		{ return bytes_; }
	iterator end() 			{ return bytes_ + length; };
	const_iterator begin() const 	{ return bytes_; }
	const_iterator end() const 	{ return bytes_ + length; };

	template <typename T> 
	void assign( T from ) 
	{
		std::copy( from, from + length, bytes_ );
	}

private:
	void copy_from( const Guid& );
	char bytes_[ length ];
};

bool operator<( const Guid&, const Guid& );
bool operator==( const Guid&, const Guid& );
bool operator!= (const Guid&, const Guid& );

#endif //GUID_HPP
