/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2010  Andrey Stroganov <savthe@gmail.com>

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

#include <boost/system/system_error.hpp>
#include <boost/static_assert.hpp>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <iostream>
#include <assert.h>

#include "asyncfile.hpp"

using namespace fio;
using namespace boost::system;

AsyncFile::AsyncFile():
	fd_(-1),
	mode_(NONE)
{
}

void AsyncFile::open(const std::string& fileName, fio::mode_t mode)
{
	BOOST_STATIC_ASSERT(sizeof(off_t) >= 8);

	int posixFlags = 0;
	const ::mode_t md = S_IRUSR | S_IWUSR;

	if(opened()) throw std::logic_error("File is already opened");

	switch (mode)
	{
		case READ: posixFlags = O_RDONLY; break;
		case WRITE: posixFlags = O_WRONLY | O_CREAT; break;
		case RW: posixFlags = O_RDWR | O_CREAT; break;
		default: throw std::logic_error("Bad file mode");
	}

	fd_ = ::open(fileName.c_str(), posixFlags, md);

	if(fd_ != -1) mode_ = mode;
	else throw system_error(errno, system_category());
}

int64 AsyncFile::size()
{
	const int64 offset = lseek(fd_, 0, SEEK_CUR);
	if(offset == -1) 
		throw system_error(errno, system_category());

	const int64 s = lseek(fd_, 0, SEEK_END);
	lseek(fd_, offset, SEEK_SET);
	return s;
}

void AsyncFile::sync()
{
	fsync(fd_);
}

AsyncFile::~AsyncFile()
{
	if(mode_ == NONE) assert(fd_ == -1);
	else
	{
		assert(fd_ != -1);
		::close(fd_);
	}
}


void AsyncFile::read(char* buffer, int64 length, int64 offset)
{
	if(pread(fd_, static_cast <void*> (buffer), length, offset) != length)
		throw system_error(errno, system_category());
}

void AsyncFile::write(const char* buffer, int64 length, int64 offset)
{
	if(pwrite(fd_, static_cast <const void*> (buffer), length, offset) != length)
		throw system_error(errno, system_category());
}

void AsyncFile::close()
{
	if(opened())
	{
		::close(fd_);
		mode_ = NONE;
		fd_ = -1;
	}
}

bool AsyncFile::opened() const
{
	return mode_ != NONE;
}

