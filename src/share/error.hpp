/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008  Andrey Stroganov <savthe@gmail.com>

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

#ifndef SHARE_ERROR_HPP
#define SHARE_ERROR_HPP

#include <stdexcept>

namespace share_details { 
namespace error {

struct AlreadyShared: public std::logic_error
{
	AlreadyShared(): std::logic_error("Item is already shared") {}
};

struct NotFound: public std::logic_error
{
	NotFound(): std::logic_error("Not found") {}
};

struct NotPermited: public std::logic_error
{
	NotPermited(const std::string& msg): std::logic_error(msg) {}
};

} //namespace error 
} //namespace share_details

#endif //SHARE_ERROR_HPP


