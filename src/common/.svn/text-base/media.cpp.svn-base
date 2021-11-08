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

#include <cstring>
#include <string>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include "media.hpp"

namespace media {

const char* exts(Type t)
{
	switch (t)
	{
		case types::video: 	return extensions::video;
		case types::audio: 	return extensions::audio;
		case types::archive: 	return extensions::archive;
		case types::msapp: 	return extensions::virus;
		case types::cdimage: 	return extensions::cdimage;
		case types::document: 	return extensions::document;
		case types::picture: 	return extensions::picture;
	}

	throw std::runtime_error("Unknown media type: " + StaticName::toString(t));
}

bool match(Type t, const std::string& ext)
{
	const std::string lowerExt = boost::to_lower_copy(ext);
	const char* extList = exts(t);
	return std::strstr(extList, lowerExt.c_str()) != NULL;
}

Type resolveByName(const std::string& name)
{
	std::string::size_type pos = name.find_last_of(".");
	if (pos >= name.size() - 1) return types::unknown;
	return resolve(name.substr(pos + 1));
}

Type resolve(const std::string& ext)
{
	if (match(types::video, ext)) 	return types::video;
	if (match(types::audio, ext)) 	return types::audio;
	if (match(types::archive, ext)) return types::archive;
	if (match(types::msapp, ext)) 	return types::msapp;
	if (match(types::cdimage, ext))	 return types::cdimage;
	if (match(types::document, ext)) return types::document;
	if (match(types::picture, ext)) return types::picture;

	return types::unknown;
}

} //namespace media

