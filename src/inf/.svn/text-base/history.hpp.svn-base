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

#ifndef NSINF_HISTORY_HPP
#define NSINF_HISTORY_HPP

#include <cstddef>

#include "ip.hpp"
#include "types.hpp"
#include "byterange.hpp"

#include "../fileinfo.hpp"
#include "../download/states.hpp"

namespace inf {
namespace history {

struct Download
{
	Download(): size(0) {}
	Download(const std::string& p, file_offset_t s): path(p), size(s) {}
	std::string path;
	file_offset_t size;
};

struct Upload
{
	Upload() {}
	Upload(const std::string& nick_, const std::string& object_, 
			const std::string& client_, uint64 transferred_, ip::Endpoint endpoint_):
		nick(nick_), object(object_), client(client_), transferred(transferred_), endpoint(endpoint_) {}

	std::string nick;
	std::string object;
	std::string client;
	uint64 transferred;
	ip::Endpoint endpoint;
};

struct RecentVersion
{
	RecentVersion() {}
	RecentVersion(unsigned int b, const std::string& t, const std::string& l, const std::string& c):
		birthday(b), tag(t), link(l), comments(c) {}

	unsigned int birthday;
	std::string tag;
	std::string link;
	std::string comments;
};

} // namespace history
} // namespace inf

#endif // NSINF_HISTORY_HPP 

