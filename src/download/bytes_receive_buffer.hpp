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

#ifndef DL_BYTES_RECEIVE_BUFFER
#define DL_BYTES_RECEIVE_BUFFER

#include <cstddef>
#include <vector>

#include "byterange.hpp"

namespace download_details {

class BytesReceiveBuffer
{
public:
	typedef std::vector<char> Bytes;
	typedef Bytes::const_iterator iterator;
	BytesReceiveBuffer(): offset_(0) {}

	void put(const char*, std::size_t);
	void range(ByteRange r);
	ByteRange range() const { return range_; }
	std::size_t size() const { return bytes_.size(); }

	const char* bytes() const { return bytes_.empty() ? 0 : &bytes_[0]; }
	iterator begin() const { return bytes_.begin(); }
	iterator end() const { return bytes_.end(); }

private:
	std::vector<char> bytes_;
	std::size_t offset_;
	ByteRange range_;
};

} // namespace download_details

#endif // DL_BYTES_RECEIVE_BUFFER

