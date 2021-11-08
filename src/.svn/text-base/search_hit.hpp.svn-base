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

#ifndef SEARCH_HIT_HPP
#define SEARCH_HIT_HPP

#include <string>

#include "types.hpp"
#include "hashes.hpp"

struct SearchHit
{
	SearchHit(): available(false), partial(0), id(0), size(0), group(0){}

	bool available;
	uint32 partial;
	uint32 id;
	file_offset_t size;
	char group;
	hash::SHA1 sha1;
	hash::MD5 md5;
	hash::TTR ttr;
	hash::ED2K ed2k;
	std::string url;
	std::string dn;
	std::string meta;
	uint16 cachedSources;
	std::string comment;
};

#endif //SEARCH_HIT_HPP

