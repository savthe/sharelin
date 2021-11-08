/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012  Andrey Stroganov <savthe@gmail.com>

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

#ifndef FIO_ERROR_HPP
#define FIO_ERROR_HPP

#include <boost/system/system_error.hpp>
#include <boost/system/error_code.hpp>

namespace fio {
namespace error {

struct SystemError: public boost::system::system_error
{	    
	SystemError(int e, const char* m): 
		boost::system::system_error(e, boost::system::system_category(), m) {}
};

struct AlreadyOpened: public std::logic_error
{
	AlreadyOpened(): std::logic_error("File is already opened") {}
};

struct BadMode: public std::logic_error
{
	BadMode(): std::logic_error("Bad open mode") {}
};

} // namespace error
} // namespace fio

#endif // FIO_ERROR_HPP


