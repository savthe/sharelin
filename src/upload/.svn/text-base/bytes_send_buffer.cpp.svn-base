/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2013  Andrey Stroganov <savthe@gmail.com>

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

#include "bytes_send_buffer.hpp"

namespace upload_details {

void BytesSendBuffer::get(char* buf, std::size_t len)
{
	if(offset_ + len > bytes_.size()) throw std::logic_error("BytesSendBuffer::put size error");

	std::copy(bytes_.begin() + offset_, bytes_.begin() + offset_ + len, buf);
	offset_ += len;
}

} // namespace upload_details

