/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012 Andrey Stroganov <savthe@gmail.com>

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

#ifndef DL_HTTP_TASK_HPP
#define DL_HTTP_TASK_HPP

#include <string>

#include "byterange.hpp"

namespace download_details {

struct HttpTask
{
	enum Type {NONE, FETCH_FILE, FETCH_TIGER};

	HttpTask(): type(NONE) {}
	HttpTask(Type t, const std::string& uri_): type(t), uri(uri_) {}

	Type type;
	std::string uri;
	ByteRange range;
	std::string accept_type;
};

} // namespace download_details

#endif // DL_HTTP_TASK_HPP

