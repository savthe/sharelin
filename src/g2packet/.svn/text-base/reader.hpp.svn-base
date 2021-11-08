/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov

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

#ifndef READER_HPP
#define READER_HPP

#include <cassert>

#include "error.hpp"
#include "packet.hpp"
#include "header.hpp"

namespace g2 {

class Reader
{
public:
	enum OffsetType { root, payload };

	explicit Reader(const char* data, uint length, OffsetType off = root);
	explicit Reader(const Packet& pk, OffsetType off = root);
	Reader(const Reader&, OffsetType off);
	Reader(const Reader&);

	bool next();
	void reset();
	void set(const char* data, uint length, OffsetType off = root);
	void read(char*, std::size_t);
	void advance(std::size_t);
	void seekPayload();

	PacketType type() const;
	Reader child() const;
	const char* data() const; 
	std::size_t left() const;
	bool contains(PacketType) const;

	template <typename T> T pod()
	{
		T val;
		//BOOST_STATIC_ASSERT(boost::is_pod<T>::value);
		read(reinterpret_cast<char*>(&val), sizeof(T));
		return val;
	}

private:
	bool atRoot() const { return offset_ == 0; }
	bool atChild() const { return compound_ && offset_; }
	bool atPayload() const { return !(atChild() || atRoot()); }
	inline void stateAssert(bool b) const { if (!b) throw BadState(); }

	inline void dataAdvance(unsigned int n)
	{
		if(offset_ + n > size_) throw BadPacket("offset > size");
		offset_ += n;
	}

	const char* data_;
	uint size_;
	uint offset_;
	bool compound_;
	Header header_;
};

} //namespace G2

#endif //READER_HPP


