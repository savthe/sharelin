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

#ifndef MEDIA_HPP
#define MEDIA_HPP

#include "staticname.hpp"
#include "types.hpp"

namespace media {

typedef uint64 Type;

namespace types {
	const Type video = StaticName::Encoder <'v', 'i', 'd', 'e', 'o'>::result;
	const Type audio = StaticName::Encoder <'a', 'u', 'd', 'i', 'o'>::result;
	const Type unknown = StaticName::Encoder <'u', 'n', 'k', 'n', 'o', 'w', 'n'>::result;
	const Type archive = StaticName::Encoder <'a', 'r', 'c', 'h', 'i', 'v', 'e'>::result;
	const Type msapp = StaticName::Encoder <'m', 's', 'a', 'p', 'p'>::result;
	const Type cdimage = StaticName::Encoder <'c', 'd', 'i', 'm', 'a', 'g', 'e'>::result;
	const Type document = StaticName::Encoder <'d', 'o', 'c', 'u', 'm', 'e', 'n', 't'>::result;
	const Type picture = StaticName::Encoder <'p', 'i', 'c', 't', 'u', 'r', 'e'>::result;
} // namespace types

namespace extensions {
	const char video[] = "avi mp4 mpg mpeg ogm mkv";
	const char audio[] = "wav mp3 ogg mid";
	const char archive[] = "rar zip 7z cab";
	const char virus[] = "exe com"; // most likely =)
	const char cdimage[] = "iso nrg bin";
	const char document[] = "pdf doc txt chm docx odt rtf";
	const char picture[] = "bmp psd gif png jpg jpeg xcf";
} // namespace extensions

const char* exts(Type t);

bool match(Type t, const std::string& ext);
Type resolve(const std::string&);
Type resolveByName(const std::string&);

} //namespace media

#endif //MEDIA_HPP

