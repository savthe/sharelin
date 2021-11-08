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

#ifndef FILE_HPP
#define FILE_HPP

#include <stdexcept>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "fileinfo.hpp"
#include "asyncfile.hpp"

class File: public boost::noncopyable
{
public:
	typedef boost::function<void(Id)> CacheNotifier;

	File(const FileInfo&, CacheNotifier);

	void create();
	void read(char* buffer, int64 length, int64 offset);
	void write(const char* buffer, int64 length, int64 offset);
	void relink(const std::string&);
	void finalize_partial(const std::string&);
	void sync();
	void shutdown();

	file_offset_t size() const { return info_.size; }

	void increase_requests() { ++info_.requests; }
	void increase_hits() { ++info_.hits; }

	void tiger_tree(const TigerTree& t) { info_.tiger_tree = t; }
	const TigerTree& tiger_tree() const { return info_.tiger_tree; }

	void partial(Id id) { info_.partial = id; }
	Id partial() const { return info_.partial; }

	void set_valid(ByteRange r) { info_.map.set_valid(r); }
	void set_invalid(ByteRange r) { info_.map.set_invalid(r); }

	void scanned(const hash::SHA1&, const TigerTree&, const hash::MD5&, const hash::ED2K&, file_offset_t);
	void resolve_hashes(const hash::SHA1&, const hash::TTR&, const hash::MD5&, const hash::ED2K&);
	void sha1(const hash::SHA1&);
	void ttr(const hash::TTR&);
	void md5(const hash::MD5&);
	void ed2k(const hash::ED2K&);
	void name(const std::string&);
	hash::SHA1 sha1() const { return info_.sha1; }
	hash::TTR ttr() const { return info_.ttr; }
	hash::MD5 md5() const { return info_.md5; }
	hash::ED2K ed2k() const { return info_.ed2k; }

	std::string path() const { return info_.path; }
	std::string title() const { return info_.title(); }
	std::string name() const { return info_.name; }
	const FileMap& map() const { return info_.map; }

	const FileInfo& info() const { return info_; }
	//FileInfo& info() { return info_; }

private:
	friend class SharedFile;

	void open();
	void close();

	inline AsyncFile& impl()
	{
		if(impl_) return *impl_;
		throw std::logic_error("File is virtual");
	}

	void recache();

	boost::scoped_ptr<AsyncFile> impl_;
	FileInfo info_;
	std::size_t refs_;
	CacheNotifier notify_;
};

#endif //FILE_HPP

