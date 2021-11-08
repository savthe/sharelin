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

#ifndef UPLOAD_FILE_SEND_BUFFER
#define UPLOAD_FILE_SEND_BUFFER

#include <cstddef>

#include "sharedfile.hpp"
#include "byterange.hpp"

namespace upload_details {
	
class FileSendBuffer
{
public:
	FileSendBuffer(): offset_(0) {}

	void reset(SharedFile, ByteRange);
	void get(char*, std::size_t);
	file_offset_t remain() const { return range_.length() - offset_; }
	ByteRange range() const { return range_; }

private:
	SharedFile file_;
	ByteRange range_;
	std::size_t offset_;
};

} // namespace upload_details

#endif // UPLOAD_FILE_SEND_BUFFER

