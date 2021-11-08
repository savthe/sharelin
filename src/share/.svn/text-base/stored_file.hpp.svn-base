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

#ifndef STORED_FILE_HPP
#define STORED_FILE_HPP

#include <string>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "hashes.hpp"
#include "../fileinfo.hpp"
#include "types.hpp"

class File;

namespace share_details {

class StoredFile
{
public:
	typedef boost::shared_ptr<File> FilePtr;
	typedef boost::function<void(Id)> Notifier;

	StoredFile() {}
	StoredFile(const FileInfo&, Notifier);

	std::string path() const;
	hash::SHA1 sha1() const;
	hash::TTR ttr() const;
	hash::MD5 md5() const;
	hash::ED2K ed2k() const;
	Id id() const;
	std::time_t hashed() const;
	Id partial() const;
	const FileInfo& info() const;
	void scanned(const hash::SHA1&, const TigerTree&, const hash::MD5&, const hash::ED2K&, file_offset_t);

	FilePtr get() const;

private:
	FilePtr file_;
};

bool operator<(const StoredFile& f1, const StoredFile& f2);
bool operator==(const StoredFile& f1, const StoredFile& f2);
bool operator!=(const StoredFile& f1, const StoredFile& f2);

} // namespace share_details

#endif // STORED_FILE_HPP
