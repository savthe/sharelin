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

#include "file.hpp"
#include "util.hpp"
#include "debug.hpp"

using util::now;

File::File(const FileInfo& info, CacheNotifier notify):
	impl_(new AsyncFile), info_(info), refs_(0), notify_(notify)
{
	if(info_.size == 0) 
		info_.hashed = 0;

	if(info_.partial)
	{
		if(info_.map.empty() || info_.size != info_.map.file_size())
		{
			info_.map.clear();
			info_.map.init(info_.size);
		}
	}
	else
		info_.map.clear();
}

void File::resolve_hashes(const hash::SHA1& hsha1, const hash::TTR& httr, const hash::MD5& hmd5, const hash::ED2K& hed2k)
{
	if(info_.sha1.empty()) info_.sha1 = hsha1;
	if(info_.ttr.empty()) info_.ttr = httr;
	if(info_.md5.empty()) info_.md5 = hmd5;
	if(info_.ed2k.empty()) info_.ed2k = hed2k;
	recache();
}

void File::scanned(const hash::SHA1& hsha1, const TigerTree& htt, const hash::MD5& hmd5, const hash::ED2K& hed2k, file_offset_t sz)
{
	assert(!htt.empty());
	info_.sha1 = hsha1;
	info_.ttr = htt.root();
	info_.md5 = hmd5;
	info_.ed2k = hed2k;
	info_.size = sz;
	info_.hashed = now();
	info_.tiger_tree = htt;
	recache();
}

void File::finalize_partial(const std::string& f)
{
	assert(info_.partial);
	info_.partial = 0;
	info_.map.clear();

	relink(f);
}

void File::name(const std::string& s)
{
	info_.name = s;
	recache();
}


void File::sha1(const hash::SHA1& h) 
{ 
	if(!info_.sha1.empty())
		throw std::logic_error("Hash is already available");
	info_.sha1 = h;
	recache();
}

void File::ttr(const hash::TTR& h) 
{ 
	if(!info_.ttr.empty())
		throw std::logic_error("Hash is already available");
	info_.ttr = h;
	recache();
}

void File::md5(const hash::MD5& h) 
{ 
	if(!info_.md5.empty())
		throw std::logic_error("Hash is already available");
	info_.md5 = h;
	recache();
}

void File::ed2k(const hash::ED2K& h) 
{ 
	if(!info_.ed2k.empty())
		throw std::logic_error("Hash is already available");
	info_.ed2k = h;
	recache();
}

void File::create()
{
	if(impl().opened()) throw std::logic_error("File is already opened");

	AsyncFile f;
	f.open(info_.path, fio::RW);
	if(info_.size)
	{
		char tmp = 0;
		f.write(&tmp, 1, info_.size - 1);
		info_.map.clear();
		info_.map.init(info_.size);
	}
	f.close();
}

void File::relink(const std::string& path)
{
	info_.path = path;

	if(impl().opened())
	{
		impl().close();
		const fio::mode_t mode = info_.partial ? fio::RW : fio::READ;
		impl().open(info_.path, mode);
	}

	recache();
}

void File::shutdown()
{
	impl().close();
	impl_.reset();
}

void File::sync()
{
	impl().sync();
}

void File::recache()
{
	notify_(info_.id);
}

void File::open()
{
	if(refs_ == 0)
	{
		const fio::mode_t mode = info_.partial ? fio::RW : fio::READ;
		impl().open(info_.path, mode);
	}

	++refs_;
}

void File::close()
{
	if(refs_ == 0) throw std::logic_error("file is closed");

	if(--refs_ == 0)
		impl().close();
}

void File::read(char* buffer, int64 length, int64 offset)
{
	if(info_.partial)
	{
		Integrity i = info_.map.status(ByteRange(offset, length));
		if(i != Integrity::FULL && i != Integrity::VALID)
			throw std::logic_error("Attemt to read invalid part of file");
	}

	return impl().read(buffer, length, offset);
}

void File::write(const char* buffer, int64 length, int64 offset)
{
	if(!info_.partial)
		throw std::logic_error("File write error. File is not partial");

	Integrity i = info_.map.status(ByteRange(offset, length));
	if(i.value() & Integrity::VALID || i.value() & Integrity::FULL)
		throw std::logic_error("Attemt to write on data");

	impl().write(buffer, length, offset);
	info_.map.set_full(ByteRange(offset, length));
}


