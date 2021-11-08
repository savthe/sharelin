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

#ifndef SHAREDFILE_HPP
#define SHAREDFILE_HPP

#include <stdexcept>

#include <boost/weak_ptr.hpp>

#include "file.hpp"

class SharedFile
{
public:
	typedef boost::shared_ptr<File> FilePtr;

	struct LockFailed: public std::logic_error
	{
		LockFailed(): std::logic_error("Couldn't lock file info") {}
	};

	SharedFile(): opened_(false) {}
	SharedFile(FilePtr p): file_(p), opened_(false) {}
	SharedFile(const SharedFile&);
	SharedFile& operator=(const SharedFile&);
	~SharedFile();

	void open();
	void close();

	bool expired() const { return file_.expired(); }
	operator bool() const { return !expired(); };

	void release();

	File* operator->() const { return lock().get(); }
	File& operator*() const { return *lock().get(); }


	inline FilePtr lock() const
	{
		if (!file_.lock()) throw LockFailed();

		return file_.lock();
	}


private:
	boost::weak_ptr<File> file_;
	bool opened_;
};

#endif //SHAREDFILE_HPP
