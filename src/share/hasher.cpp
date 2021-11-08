/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011 Andrey Stroganov <savthe@gmail.com>

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

#include <stdexcept>
#include <iomanip>
#include <cassert>

#include <boost/bind.hpp>

#include "hasher.hpp"
#include "storage.hpp"
#include "localqht.hpp"
#include "verb.hpp"
#include "filesystem.hpp"
#include "util.hpp"
#include "task_server.hpp"
#include "debug.hpp"

namespace share_details {

Hasher::Hasher(Storage& s, LocalQht& q):
	ahash_(app::task_server()),
	storage_(s),
	qht_(q),
	rate_(0)
{
}

Hasher::~Hasher()
{
}

void Hasher::run(const std::string& path)
{
	if(ahash_.active()) throw std::logic_error("Hasher is busy");

	//if(!w) throw std::logic_error("Weak info is invalid");

	verb2::shr << "Running async hash for: " << path << std::endl;

	ahash_.run(path, hash::Mask::ANY, ByteRange(0, FileSystem::FileSize(path)), rate_, boost::bind(&Hasher::done, this, _1));

	path_ = path;
}

void Hasher::done(const boost::system::error_code& err)
{
	StoredFile f;

	try
	{
		f = storage_.get<tag::path>(path_);
	}
	catch (error::NotFound&) 
	{
		verb1::shr << "File " << path_ << " is no longer shared." << std::endl;
		return;
	}

	if(err)
	{
		verb1::shr << "There was an error during file scan: " << err.message() << ". File will be removed from share." << std::endl;
		storage_.remove(f.id());
	}
	else
	{
		verb1::shr << "File " << f.path() << " hashed. Rate: " << std::setprecision(2) << 
			double(ahash_.rate()) / (1024 * 1024) << " Mbps" << std::endl;

		const file_offset_t size = ahash_.range().length();

		hash::TTR root(ahash_.tiger_tree().root());
		assert(ahash_.ttr() == root);

		f.scanned(ahash_.sha1(), ahash_.tiger_tree(), ahash_.md5(), ahash_.ed2k(), size);
		qht_.update(f.info());
	}
}

} //namespace share_details

