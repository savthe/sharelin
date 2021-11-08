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

#ifndef NSINF_UPLOAD_HPP
#define NSINF_UPLOAD_HPP

#include <string>
#include <vector>

#include "types.hpp"
#include "ip.hpp"

namespace inf {

struct Uploader
{
	ip::Endpoint endpoint;
	std::string nick;
	std::string title;
	file_offset_t sent;
	file_offset_t rate;
	std::string client;
	bool queued;
};

struct UploadManager
{
	std::vector<Uploader> uploaders;
};


} // namespace inf

#endif // NSINF_UPLOAD_HPP 

