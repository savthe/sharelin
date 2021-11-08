/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012 Andrey Stroganov <savthe@gmail.com>

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

#include <boost/bind.hpp>

#include "validator.hpp"
#include "filemap.hpp"
#include "verb.hpp"
#include "task_server.hpp"
#include "io.hpp"

using boost::bind;

namespace download_details {

Validator::Validator(SharedFile& f):  
	file_(f),
	ahash_(app::task_server()),
	ticker_(app::io())
	{}

void Validator::tick()
{
	ticker_.tick(boost::posix_time::seconds(5), bind(&Validator::tick, this));
	if(!file_) return;
	if(file_->tiger_tree().empty()) return;
	if(ahash_.active()) return;

	validate();
}

void Validator::on_hashed(const boost::system::error_code& err)
{
	ByteRange r = ahash_.range();

	if(err)
	{
		verb1::dl << "ERROR during part hash " << r.first() << "-" << r.last() << std::endl;
		return;
	}

	if( ahash_.ttr() == hash::TTR(file_->tiger_tree().get(r.offset(), r.length())) )
		file_->set_valid(r);
	else
	{
		verb2::dl << "Invalidating " << file_->path() << " Range: " << r.first() << "-" << r.last() << std::endl;
		file_->set_invalid(r);
	}

	validate();
}

void Validator::start()
{
	ticker_.tick(boost::posix_time::seconds(1), bind(&Validator::tick, this));
}

void Validator::stop()
{
	ahash_.cancel();
	ticker_.cancel();
}

void Validator::validate()
{
	ByteRange r = select_range();
	if(r.empty()) return;

	ahash_.run(file_->path(), hash::Mask::TTR, r, 0, bind(&Validator::on_hashed, this, _1));
}

ByteRange Validator::select_range() const
{
	const file_offset_t versize = file_->tiger_tree().block_size();

	ByteRange r;

	for(FileMap::iterator i = file_->map().begin(); r.empty() && i != file_->map().end(); ++i)
	{
		if(i->color() != Integrity::FULL) continue;

		file_offset_t offset = i->first();
		if(offset % versize) offset += versize - offset % versize;
		assert(offset % versize == 0);
		if(offset > i->last()) continue;

		FileMap::iterator next = i;
		++next;
		file_offset_t length = (next == file_->map().end() ? i->last() - offset + 1 : versize);
		length = std::min(length, versize);

		if(offset + length - 1 <= i->last())
			r.set(offset, length);
	}

	return r;
}

} // namespace download_details

