/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009  Andrey Stroganov

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

#ifndef ZLIB_HPP
#define ZLIB_HPP

#include <stdexcept>
#include <vector>

namespace ZLib
{
	typedef std::vector <char> ByteVector;
	struct Error: public std::runtime_error
	{
		explicit Error(const std::string& err): std::runtime_error(err) {}
	};

	void compress(const ByteVector& src, ByteVector& dst);
	void uncompress(const ByteVector& src, ByteVector& dst, unsigned int maxSize = 0);
	unsigned int compress(const char* from, unsigned int fromSize, char* to, unsigned int toSize);
	unsigned int uncompress(const char* from, unsigned int fromSize, char* to, unsigned int toSize);

	unsigned int compressBound(unsigned int);
	template <typename SrcIterator, typename DstIterator>
	void compress(SrcIterator begin, SrcIterator end, DstIterator dest)
	{
		ByteVector srcv(begin, end);
		ByteVector dstv;
		compress(srcv, dstv);
		std::copy(dstv.begin(), dstv.end(), dest);
	}

	template <typename SrcIterator, typename DstIterator>
	void uncompress(SrcIterator begin, SrcIterator end, DstIterator dest)
	{
		ByteVector srcv(begin, end);
		ByteVector dstv;
		uncompress(srcv, dstv);
		std::copy(dstv.begin(), dstv.end(), dest);
	}
}

#endif //ZLIB_HPP

