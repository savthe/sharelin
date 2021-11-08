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

#ifndef FILEINFO_HPP
#define FILEINFO_HPP

#include <string>
#include <ctime>

#include "hashes.hpp"
#include "types.hpp"
#include "tigertree.hpp"
#include "filemap.hpp"

struct FileInfo
{
	FileInfo(): hashed(0), hits(0), requests(0), id(0), partial(INVALID_ID), size(0) {}
	FileInfo(const std::string& p): path(p), hashed(0), hits(0), requests(0), id(0), partial(INVALID_ID), size(0) {}

	std::string title() const
	{
		if(!name.empty()) return name;
		const std::size_t i = path.find_last_of('/');
		if(i == std::string::npos) return path;
		return path.substr(i + 1);
	}
	
	std::string name;
	std::string path;
	hash::SHA1 sha1;
	hash::MD5 md5;
	hash::ED2K ed2k;
	hash::TTR ttr;
	hash::BP bp;
	std::time_t hashed;
	uint hits;
	uint requests;
	Id id;
	Id partial;
	TigerTree tiger_tree; 
	FileMap map;
	file_offset_t size;
};

#endif //FILEINFO_HPP

