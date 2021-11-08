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

#ifndef UPLOAD_UPLOADER_STATE_TIGER_HPP
#define UPLOAD_UPLOADER_STATE_TIGER_HPP

#include "uploader_state.hpp"
#include "bytes_send_buffer.hpp"

namespace upload_details {

class Uploader;

class UploaderStateTiger: public UploaderState
{
public:
	UploaderStateTiger(Uploader*);
	file_offset_t size() const; 
	bool is_found() const;
	void print_request() const;
	void on_sent();
	void process_transfer(std::ostream&);
	void start_transfer();

private:
	Uploader& uploader_;
	BytesSendBuffer buffer_;
};

} // upload_details

#endif // UPLOAD_UPLOADER_STATE_TIGER_HPP


