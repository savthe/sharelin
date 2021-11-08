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

#ifndef G2HTTPREQUEST_HPP
#define G2HTTPREQUEST_HPP

#include "byterange.hpp"

namespace g2 {

struct HttpRequest
{
	enum MethodType {GET, HEAD};
	enum ObjectType {SHARED_FILE, TIGER_TREE};

	HttpRequest(): keepAlive(false), method(GET) {}

	bool keepAlive;
	MethodType method;
	ObjectType object;
	std::string urn;
	std::string nick;
	std::string client;
	ByteRange range;

};

} //namespace G2

#endif //G2HTTPREQUEST_HPP

