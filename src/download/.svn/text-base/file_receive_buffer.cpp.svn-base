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

#include <stdexcept>

#include "file_receive_buffer.hpp"

namespace download_details {

FileReceiveBuffer::FileReceiveBuffer(SharedFile f): file_(f), offset_(0)
{
}

void FileReceiveBuffer::put(const char* buf, std::size_t len)
{
	if(offset_ + len > range_.length()) throw std::logic_error("FileReceiveBuffer::put size error");

	file_->write(buf, len, range_.offset() + file_offset_t(offset_));
	offset_ += len;
}

} // namespace download_details
