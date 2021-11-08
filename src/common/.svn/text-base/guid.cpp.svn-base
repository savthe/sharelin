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

#include <cstdlib>

#include "guid.hpp"

Guid::Guid()
{
	reset();
}

Guid::Guid( const Guid& g )
{
	copy_from(g);
}

Guid::Guid( const char* p )
{
	assign(p);
}

Guid& Guid::operator=( const Guid& g )
{
	if(&g != this)  
		copy_from( g );

	return *this;
}

void Guid::reset()
{
	std::fill(bytes_, bytes_ + length, 0);
}

bool Guid::trivial() const
{
	const char* b = bytes_;

	while(b != bytes_ + length && *b == 0) ++b;

	return b == bytes_ + length;
}

void Guid::generate()
{
	for(int i = 0; i < length; ++i)
		bytes_[i] = std::rand() & 0xFF;
}

void Guid::copy_from( const Guid& g )
{
	std::copy( g.begin(), g.end(), bytes_ );
}

bool operator<( const Guid& g1, const Guid& g2 )
{
	return std::lexicographical_compare( g1.begin(), g1.end(), g2.begin(), g2.end() );
}

bool operator==( const Guid& g1, const Guid& g2 )
{
	return std::equal( g1.begin(), g1.end(), g2.begin() );
}

bool operator!=( const Guid& g1, const Guid& g2 )
{
	return !( g1 == g2 );
}

