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

//#include <iostream>

#include "filemap.hpp"

Integrity FileMap::status() const
{
	Integrity s;

	for(Map::iterator i = map_.begin(); i != map_.end(); ++i)
		s.add(i->color().value());

	return s;
}

Integrity FileMap::status(const ByteRange& r) const
{
	Integrity s;

	for(Map::iterator i = map_.begin(); i != map_.end(); ++i)
		if(r.intersects(*i))
			s.add(i->color().value());

	return s;
}


/*
void FileMap::Print() const
{
	for(RangesContainer::const_iterator i = ranges_.begin(); i != ranges_.end(); ++i)
	{
		std::cout << "   " << i->first() << "-" << i->last() << ":";
		switch(i->status())
		{
			case integrity::NONE: std::cout << "N"; break;
			case integrity::VALID: std::cout << "V"; break;
			case integrity::INVALID: std::cout << "I"; break;
			case integrity::ACTIVE: std::cout << "A"; break;
			case integrity::EMPTY: std::cout << "E"; break;
			case integrity::FULL: std::cout << "F"; break;
		}
	}
	std::cout << std::endl;
}
*/

void FileMap::init(file_offset_t size)
{
	if(!map_.empty())
		throw std::runtime_error("map is not empty");

	file_size_ = size;

	if(size)
		map_.add(Fragment(ByteRange(0, size), Integrity::EMPTY));
}

void FileMap::modify(const ByteRange& range, Integrity::Color st)
{
	if(map_.empty())
		throw std::runtime_error("map is empty");

	if(range.last() >= file_size_)
		throw std::logic_error("Range is too large");
	assert(range.last() < file_size_);

//	debug_validate_change(range, st);

	map_.add(Fragment(range, st));
}

/*
void FileMap::debug_validate_change(const ByteRange& r, Integrity to)
{
	const Integrity from = status(r);
	bool b = true;
	if(to & Integrity::EMPTY) b = (from == Integrity::WRITE);
	else if(to & Integrity::FULL) b = (from == Integrity::WRITE);
	else if(to & Integrity::VALID) b = (from == Integrity::FULL);
	else if(to & Integrity::INVALID) b = (from == Integrity::FULL);
	else if (to & integrity::WRITE) 
		b = (from == Integrity::EMPTY || from == Integrity::INVALID || from == (Integrity::EMPTY | Integrity::INVALID));
	else b = true;

	if(b) return;

	std::cout << "File map error. Range change illegal" << std::endl;
	print();
	std::cout << "changing range " << r.first() << '-' << r.last() << " from " 
		<< from.to_string() << " to " << to.to_string() << std::endl;
	assert(!"can't continue");
}
*/

void FileMap::print() const
{
	/*
	std::cout << "Map dump:" << std::endl;
	for(iterator i = begin(); i != end(); ++i)
		std::cout << i->first() << "-" << i->last() << ":" << i->color().to_string() << "   ";
	std::cout << std::endl;
	*/
}

