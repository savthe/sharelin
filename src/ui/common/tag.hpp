/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009  Andrey Stroganov <savthe@gmail.com>

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

#ifndef TAG_HPP
#define TAG_HPP

#include <staticname.hpp>
#include <types.hpp>

namespace ui 
{
	namespace Tags
	{
		typedef uint64 Type;
		const Type home = StaticName::Encoder<'h', 'o', 'm', 'e'>::result;
		const Type dlnew = StaticName::Encoder<'d', 'l', 'n', 'e', 'w'>::result;
		const Type dl = StaticName::Encoder<'d', 'l'>::result;
		const Type dls = StaticName::Encoder<'d', 'l', 's'>::result;
		const Type dlshow = StaticName::Encoder<'d', 'l', 's', 'h', 'o', 'w'>::result;
		const Type sr = StaticName::Encoder<'s', 'r'>::result;
		const Type srs = StaticName::Encoder<'s', 'r', 's'>::result;
		const Type srshow = StaticName::Encoder<'s', 'r', 's', 'h', 'o', 'w'>::result;
		const Type ups = StaticName::Encoder<'u', 'p', 's'>::result;
		const Type set = StaticName::Encoder<'s', 'e', 't'>::result;
		const Type min = StaticName::Encoder<'m', 'i', 'n'>::result;
		const Type max = StaticName::Encoder<'m', 'a', 'x'>::result;
		const Type media = StaticName::Encoder<'m', 'e', 'd', 'i', 'a'>::result;
		const Type ext = StaticName::Encoder<'e', 'x', 't'>::result;
		const Type dn = StaticName::Encoder<'d', 'n'>::result;
		const Type show = StaticName::Encoder<'s', 'h', 'o', 'w'>::result;
		const Type stop = StaticName::Encoder<'s', 't', 'o', 'p'>::result;
		const Type sort = StaticName::Encoder<'s', 'o', 'r', 't'>::result;
		const Type sortby = StaticName::Encoder<'s', 'o', 'r', 't', 'b', 'y'>::result;
		const Type sources = StaticName::Encoder<'s', 'o', 'u', 'r', 'c', 'e', 's'>::result;
		const Type size = StaticName::Encoder<'s', 'i', 'z', 'e'>::result;
		const Type sres = StaticName::Encoder<'s', 'r', 'e', 's'>::result;
		const Type kill = StaticName::Encoder<'k', 'i', 'l', 'l'>::result;
		const Type id = StaticName::Encoder<'i', 'd'>::result;
		const Type unlink = StaticName::Encoder<'u', 'n', 'l', 'i', 'n', 'k'>::result;
		const Type expand = StaticName::Encoder<'e', 'x', 'p', 'a', 'n', 'd'>::result;
		const Type resume = StaticName::Encoder<'r', 'e', 's', 'u', 'm', 'e'>::result;
		const Type link = StaticName::Encoder<'l', 'i', 'n', 'k'>::result;
		const Type sfl = StaticName::Encoder<'s', 'f', 'l'>::result;
		const Type settings = StaticName::Encoder<'s', 'e', 't', 't', 'i', 'n', 'g', 's'>::result;
		const Type priority = StaticName::Encoder<'p', 'r', 'i', 'o', 'r', 'i', 't', 'y'>::result;
		const Type incoming = StaticName::Encoder<'i', 'n', 'c', 'o', 'm', 'i', 'n', 'g'>::result;
		const Type log = StaticName::Encoder<'l', 'o', 'g'>::result;
		const Type share = StaticName::Encoder<'s', 'h', 'a', 'r', 'e'>::result;
		const Type adl = StaticName::Encoder<'a', 'd', 'l'>::result;
		const Type name = StaticName::Encoder<'n', 'a', 'm', 'e'>::result;
		const Type done = StaticName::Encoder<'d', 'o', 'n', 'e'>::result;
		const Type rate = StaticName::Encoder<'r', 'a', 't', 'e'>::result;
		const Type selected = StaticName::Encoder<'s', 'e', 'l', 'e', 'c', 't', 'e', 'd'>::result;
		const Type page = StaticName::Encoder<'p', 'a', 'g', 'e'>::result;
		const Type requests = StaticName::Encoder<'r', 'e', 'q', 'u', 'e', 's', 't', 's'>::result;
		const Type hits = StaticName::Encoder<'h', 'i', 't', 's'>::result;
		const Type pgreload = StaticName::Encoder<'p', 'g', 'r', 'e', 'l', 'o', 'a', 'd'>::result;
		const Type status = StaticName::Encoder<'s', 't', 'a', 't', 'u', 's'>::result;
		const Type purge = StaticName::Encoder<'p', 'u', 'r', 'g', 'e'>::result;
		const Type srchs = StaticName::Encoder<'s', 'r', 'c', 'h', 's'>::result;
		const Type ps = StaticName::Encoder<'p', 's'>::result;
		const Type p = StaticName::Encoder<'p'>::result;
		const Type srdel = StaticName::Encoder<'s', 'r', 'd', 'e', 'l'>::result;
		const Type dlinfo = StaticName::Encoder<'d', 'l', 'i', 'n', 'f', 'o'>::result;
		const Type dlpri = StaticName::Encoder<'d', 'l', 'p', 'r', 'i'>::result;
		const Type dlstop = StaticName::Encoder<'d', 'l', 's', 't', 'o', 'p'>::result;
		const Type dlresume = StaticName::Encoder<'d', 'l', 'r', 'e', 's', 'u', 'm', 'e'>::result;
		const Type dldel = StaticName::Encoder<'d', 'l', 'd', 'e', 'l'>::result;
		const Type dlgroup = StaticName::Encoder<'d', 'l', 'g', 'r', 'o', 'u', 'p'>::result;
		const Type nb = StaticName::Encoder<'n', 'b'>::result;
		const Type r = StaticName::Encoder<'r'>::result;
		const Type dlhist = StaticName::Encoder<'d', 'l', 'h', 'i', 's', 't'>::result;
		const Type uplhist = StaticName::Encoder<'u', 'p', 'l', 'h', 'i', 's', 't'>::result;
		const Type uiset = StaticName::Encoder<'u', 'i', 's', 'e', 't'>::result;
		const Type uivar = StaticName::Encoder<'u', 'i', 'v', 'a', 'r'>::result;
		const Type uival = StaticName::Encoder<'u', 'i', 'v', 'a', 'l'>::result;
		const Type redirect = StaticName::Encoder<'r', 'e', 'd', 'i', 'r', 'e', 'c', 't'>::result;
		const Type sync = StaticName::Encoder<'s', 'y', 'n', 'c'>::result;
		const Type debug = StaticName::Encoder<'d', 'e', 'b', 'u', 'g'>::result;
	} // namespace Tags
} // namespace ui

#endif //TAG_HPP


