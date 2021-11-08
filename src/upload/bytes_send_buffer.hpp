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

#ifndef UPLOAD_BYTES_SEND_BUFFER
#define UPLOAD_BYTES_SEND_BUFFER

#include <cstddef>
#include <vector>
#include <algorithm>

namespace upload_details {
	
class BytesSendBuffer
{
public:
	BytesSendBuffer(): offset_(0) {}

	template<typename I> void reset(I begin, I end)
	{
		offset_ = 0;
		bytes_.clear();
		std::copy(begin, end, std::back_inserter(bytes_));
	}

	void get(char* buf, size_t len);
	size_t remain() const { return bytes_.size() - offset_; }

private:
	size_t offset_;
	std::vector<char> bytes_;
};

} // namespace upload_details

#endif // BYTES_FILE_SEND_BUFFER

