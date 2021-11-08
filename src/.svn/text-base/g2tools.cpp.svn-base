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

#include "g2tools.hpp"

namespace g2 {

void extractUrn(g2::Reader r, hash::SHA1& sha1, hash::TTR& ttr, hash::MD5& md5, hash::ED2K& ed2k)
{
	r.seekPayload();

	if(r.left() < 5) return;

	const char* p = r.data();
	const uint sz = r.left();

	if(std::memcmp(p, "sha1\0", 5) == 0 && sz == 5 + hash::SHA1::size())
 		sha1.assign(p + 5);

	else if(std::memcmp(p, "ttr\0", 4) == 0 && sz == 4 + hash::TTR::size())
 		ttr.assign(p + 4);

	else if(std::memcmp(p, "tree:tiger/\0", 12) == 0 && sz == 12 + hash::TTR::size())
 		ttr.assign(p + 12);

	else if(std::memcmp(p, "bp\0", 3) == 0 && sz == 3 + hash::BP::size())
	{
		hash::BP bp;
		bp.assign(p + 3);
		bp.split(sha1, ttr);
	}

	else if(std::memcmp(p, "md5\0", 4) == 0 && sz == 4 + hash::MD5::size())
 		md5.assign(p + 4);

	else if(std::memcmp(p, "ed2k\0", 5) == 0 && sz == 5 + hash::ED2K::size())
 		ed2k.assign(p + 5);
}

void composeQhtReset(g2::Packet& pk, QueryHashTable::BitsSize sz)
{
	g2::Writer w(pk);

	w.Begin(g2::QHT);
	w.Pod(char(0));
	w.Pod(sz);
	w.Pod(char(1));
	w.Close(g2::QHT);
}

void composeLni(g2::Packet& pk, ip::Endpoint self, const Guid& guid, g2::VendorId vid)
{
	g2::Writer w(pk);

	w.Begin(g2::LNI);

	w.Begin(g2::NA);
	w.Pod(self.address.value);
	w.Pod(self.port);
	w.Close(g2::NA);

	w.Begin(g2::GU);
	w.Write(guid.begin(), 16);
	w.Close(g2::GU);

	w.Begin(g2::V);
	w.Pod(vid);
	w.Close(g2::V);

	w.Close(g2::LNI);
}

void composePi(g2::Packet& pk)
{
	g2::Writer w(pk);
	w.Begin(g2::PI);
	w.Close(g2::PI);
}

void addUrn( g2::Writer& w, const std::string& prefix, const char* p, uint size)
{
	w.Begin( g2::URN );
	w.Write( prefix );
	w.Pod( char(0) );
	w.Write( p, size );
	w.Close( g2::URN );
}

} //namespace g2

