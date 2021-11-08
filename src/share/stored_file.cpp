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

#include "stored_file.hpp"
#include "../file.hpp"

namespace share_details {

StoredFile::StoredFile(const FileInfo& i, Notifier n):
	file_(new File(i, n))
	{
	}


std::string StoredFile::path() const { return file_->info().path; } 
Id StoredFile::id() const { return file_->info().id; } 
hash::SHA1 StoredFile::sha1() const { return file_->info().sha1; }
hash::TTR StoredFile::ttr() const { return file_->info().ttr; }
hash::MD5 StoredFile::md5() const { return file_->info().md5; }
hash::ED2K StoredFile::ed2k() const { return file_->info().ed2k; }
std::time_t StoredFile::hashed() const { return file_->info().hashed; }
Id StoredFile::partial() const { return file_->info().partial; }
const FileInfo& StoredFile::info() const { return file_->info(); }

void StoredFile::scanned(const hash::SHA1& hsha1, const TigerTree& htt, const hash::MD5& hmd5, const hash::ED2K& hed2k, file_offset_t sz)
{
	file_->scanned(hsha1, htt, hmd5, hed2k, sz);
}

StoredFile::FilePtr StoredFile::get() const { return file_; }
bool operator<(const StoredFile& f1, const StoredFile& f2)
{
	return f1.get() < f2.get();
}

bool operator==(const StoredFile& f1, const StoredFile& f2)
{
	return f1.get() == f2.get();
}

bool operator!=(const StoredFile& f1, const StoredFile& f2)
{
	return f1.get() != f2.get();
}

} // namespace share_details

