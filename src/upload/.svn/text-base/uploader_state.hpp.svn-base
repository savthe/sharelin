/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2013  Andrey Stroganov <savthe@gmail.com>

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

#ifndef UPLOAD_UPLOADER_STATE_HPP
#define UPLOAD_UPLOADER_STATE_HPP

#include <ostream>

#include "types.hpp"

namespace upload_details {

class UploaderState
{
public:
	virtual bool is_found() const = 0;
	virtual file_offset_t size() const = 0;
	virtual void print_request() const = 0;
	virtual void process_transfer(std::ostream&) = 0;
	virtual void start_transfer() = 0;
	virtual void on_sent() = 0;

	virtual ~UploaderState() {}
};

} // upload_details

#endif // UPLOAD_UPLOADER_STATE_HPP

