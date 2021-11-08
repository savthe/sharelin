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

#include "file_send_buffer.hpp"

namespace upload_details {

void FileSendBuffer::reset(SharedFile f, ByteRange r)
{
	offset_ = 0;
	file_ = f;
	range_ = r;
}

void FileSendBuffer::get(char* buf, std::size_t len)
{
	if(offset_ + len > range_.length()) throw std::logic_error("FileSendBuffer::put size error");

	file_->read(buf, len, range_.offset() + file_offset_t(offset_));
	offset_ += len;
}

} // namespace upload_details
