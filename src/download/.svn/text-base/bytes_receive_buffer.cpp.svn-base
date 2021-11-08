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

#include "bytes_receive_buffer.hpp"

namespace download_details {

void BytesReceiveBuffer::range(ByteRange r)
{
	offset_ = 0;
	range_ = r;
	bytes_.resize(r.length());
}

void BytesReceiveBuffer::put(const char* buf, std::size_t len)
{
	if(offset_ + len > bytes_.size()) throw std::logic_error("BytesReceiveBuffer::put size error");
	std::copy(buf, buf + len, bytes_.begin() + offset_);

	offset_ += len;
}

} // namespace download_details
