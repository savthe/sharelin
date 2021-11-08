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

#ifndef UPLOADFILEBUFFER_HPP
#define UPLOADFILEBUFFER_HPP

#include "uploadbuffer.hpp"
#include "sharedfile.hpp"

class UploadFileBuffer: public UploadBuffer
{
public:
	UploadFileBuffer(SharedFile f, ByteRange r): UploadBuffer(r), file_(f) {}
	virtual ~UploadFileBuffer() {}
	
protected:
	virtual void PopulateBuffer(char*, uint, file_offset_t);

	SharedFile file_;
};


#endif //UPLOADFILEBUFFER_HPP

