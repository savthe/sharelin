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

#ifndef NSINF_DOWNLOAD_HPP
#define NSINF_DOWNLOAD_HPP

#include <cstddef>

#include "ip.hpp"
#include "types.hpp"
#include "byterange.hpp"

#include "../download/states.hpp"
#include "../fileinfo.hpp"

namespace inf {

struct Downloader
{
	bool pushed;
	ip::Endpoint endpoint;
	download_details::dlr_state::state_t state;
	ByteRange range;
	file_offset_t total;
	std::size_t rate;
	std::size_t q_pos;
	std::size_t q_length;
};

struct Download
{
	FileInfo f;
	std::vector<Downloader> downloaders;
	std::size_t rate;
	std::size_t sources_count;
	int priority;
	download_details::dl_state::state_t state;
};

struct DownloadManager
{
	std::vector<Id> downloads;
};


} // namespace inf

#endif // NSINF_DOWNLOAD_HPP 

