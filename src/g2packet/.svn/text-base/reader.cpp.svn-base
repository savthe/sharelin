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

#include "reader.hpp"
#include <cassert>
#include <cstring>

namespace g2
{

	/*
void Reader::SkipTrivialRoot()
{
	//if offset_ == 0 we asume that this packet has no children and advancing to payload
	if(offset_ == 0) next();
//	assert(!AtPacket()); //For debugging
	if(atPacket()) throw std::runtime_error("Unexpected child packet");
}
*/

PacketType Reader::type() const
{
	stateAssert(atRoot() || atChild());

	return header_.type;
}

Reader::Reader(const Reader& r, OffsetType off) 
{ 
	set(r.data_, r.size_, off); 
}

Reader::Reader(const char* data, uint length, OffsetType off) 
{ 
	set(data, length, off); 
}

Reader::Reader(const Packet& pk, OffsetType off) 
{ 
	set(pk.Bytes(), pk.Size(), off); 
}

Reader::Reader(const Reader& r)
{
	*this = r;
}

const char* Reader::data() const
{
	stateAssert(atPayload());

	return data_ + offset_;
}

void Reader::seekPayload()
{
	stateAssert(atRoot() || atChild());

	while (next());

	assert(offset_);
}

std::size_t Reader::left() const
{
	stateAssert(atPayload());

	return size_ - offset_;
}

bool Reader::next()
{
	stateAssert(atRoot() || atChild());

	if(offset_ == size_) return false;
	if(offset_ == 0) dataAdvance(header_.headerSize);
	else if(compound_) dataAdvance(header_.packetSize);
	else return false;

	assert(offset_ <= size_);
	if(offset_ == size_) return false;
	if(offset_ && compound_ && *(data_ + offset_) == 0)
	{
		dataAdvance(1);
		assert(offset_ <= size_);
		compound_ = false;
	}
	
	assert(offset_ <= size_);

	if(compound_) header_.Decode(data_ + offset_, size_ - offset_);

	return compound_;
}

void Reader::advance(std::size_t length)
{
	stateAssert(atPayload());

	dataAdvance(length);
}

void Reader::read(char* out, std::size_t length)
{
	stateAssert(atPayload());

	if(left() < length) 
		throw std::range_error("bad range");

	std::memcpy(out, data(), length);

	advance(length);
}

Reader Reader::child() const
{
	stateAssert(atChild());

	assert(offset_ <= size_);
	Reader r(data_ + offset_, size_ - offset_);

	return r;
}

bool Reader::contains(PacketType t) const
{
	Reader r(data_, size_);

	while(r.next() && r.type() != t);

	return r.atChild();
}

void Reader::reset() 
{ 
	set(data_, size_);
}

void Reader::set(const char* data, uint length, OffsetType off)
{
	data_ = data;
	offset_ = 0;

	header_.Decode(data, length);

	size_ = header_.packetSize;
	if(size_ > length) throw std::range_error("No space for packet");
	compound_ = header_.compound;

	if(off == payload) seekPayload();
}

} //namespace G2
