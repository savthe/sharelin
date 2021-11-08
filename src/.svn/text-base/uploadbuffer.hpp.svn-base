/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011  Andrey Stroganov <savthe@gmail.com>

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

#ifndef UPLOADBUFFER_HPP
#define UPLOADBUFFER_HPP

#include <stdexcept>
#include <vector>
#include <cassert>

#include "byterange.hpp"
#include "types.hpp"

class UploadBuffer
{
	public:
	typedef std::vector<char> Container;
	UploadBuffer(ByteRange r): range_(r), globalOffset_(r.offset()), localOffset_(0), reserved_(0) {}
	ByteRange GetRange() const { return range_; }
	file_offset_t Offset() const { return globalOffset_ + localOffset_; }
	bool AtEnd() const { return Offset() == range_.offset() + range_.length(); }
	uint BytesLeft() const { return range_.offset() + range_.length() - globalOffset_ - localOffset_; }
	void Reserve(unsigned int reserved) { reserved_ = reserved; }
	virtual ~UploadBuffer() {}

	template <typename T>
	void Read(T begin, uint len)
	{
		if(len > BytesLeft()) throw std::range_error("UploadBuffer::Read");
		reserved_ = std::max(reserved_, len);
		if(localOffset_ + len > buffer_.size()) UpdateBuffer();
		assert(localOffset_ + len <= buffer_.size());
		std::copy(buffer_.begin() + localOffset_, buffer_.begin() + localOffset_ + len, begin);
		localOffset_ += len;
	}

	protected:
	virtual void PopulateBuffer(char*, uint, file_offset_t) = 0;

	private:
	void UpdateBuffer();
	Container buffer_;
	ByteRange range_;
	file_offset_t globalOffset_;
	unsigned int localOffset_;
	unsigned int reserved_;
};

#endif //UPLOADBUFFER_HPP

