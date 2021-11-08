/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov <savthe@gmail.com>

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

#include "sharedfile.hpp"

SharedFile::SharedFile(const SharedFile& f): opened_(false)
{
	file_ = f.file_;
	if(f.opened_) open();
}

SharedFile& SharedFile::operator=(const SharedFile& f)
{
	if(&f != this)
	{
		close();

		file_ = f.file_;
		if(f.opened_) open();
	}

	return *this;
}

void SharedFile::release()
{
	close();
	file_.reset();
}

void SharedFile::open()
{
	if(!opened_)
		lock()->open();
	opened_ = true;
}

void SharedFile::close()
{
	if(opened_)
	{
		if(file_.lock()) file_.lock()->close();
		opened_ = false;
	}
}

SharedFile::~SharedFile()
{
	if(opened_) close();
}

