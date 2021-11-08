/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010 Andrey Stroganov <savthe@gmail.com>

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

#ifndef G2_TOOLS_HPP
#define G2_TOOLS_HPP

#include <cstddef>

#include "g2packet.hpp"
#include "hashes.hpp"
#include "queryhashtable.hpp"
#include "zlib.hpp"
#include "g2defs.hpp"
#include "guid.hpp"
#include "ip.hpp"

namespace g2 {

void extractUrn(g2::Reader r, hash::SHA1& sha1, hash::TTR& ttr, hash::MD5& md5, hash::ED2K& ed2k);
void composeQhtReset(g2::Packet&, QueryHashTable::BitsSize);
void composeLni(g2::Packet&, ip::Endpoint, const Guid&, VendorId);
void composePi(g2::Packet&);
void addUrn(g2::Writer& w, const std::string& prefix, const char* p, uint size);

template <typename T>
void composeQht(const QueryHashTable& qht, const std::size_t packetSize, T out)
{
	std::vector<char> inverted( qht.Size() );
	uint count = 0;
	for(uint i = 0; i < qht.Size(); ++i)
	{
		if(*(qht.Bytes() + i))
			++count;
		inverted.at(i) = 0xFF - *(qht.Bytes() + i);
	}
	//std::cout << count << "/" << qht.Size() << std::endl;

	std::vector<char> compressed( ZLib::compressBound( qht.Size() ) );
	const std::size_t compressedSize = ZLib::compress( &inverted[0], qht.Size(), &compressed[0], compressed.size() ); 
	//std::cout << "QHT SIZE: " << qht.Size() << " COMPRESSED: " << compressedSize << std::endl;

	uint8 total = compressedSize / packetSize + (compressedSize % packetSize ? 1 : 0);

	assert(compressedSize / packetSize + 1 < 256);

	std::size_t offset = 0;

	for( uint8 i = 0; i < total; ++i)
	{
		g2::Packet pk;
		g2::Writer w(pk);

		w.Begin(g2::QHT);
		w.Pod(char(1)); //command
		w.Pod(char(i + 1)); //fragment number
		w.Pod(char(total));
		w.Pod(char(1)); //compressed
		w.Pod(char(1)); //g2

		const uint len = std::min(packetSize, compressedSize - offset);
		w.Write( &compressed[offset], len );
		offset += len;

		w.Close(g2::QHT);

		*out++ = pk;
	}
}

} //namespace g2

#endif //G2_TOOLS_HPP

