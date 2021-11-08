/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012  Andrey Stroganov <savthe@gmail.com>

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

#include <vector>

#include <boost/foreach.hpp>

#include "range_destribute.hpp"
#include "../filemap.hpp"
#include "downloader.hpp"

namespace download_details {

void RangeDestribute::rebuild_cache()
{
	cache_.clear();
	
	for(FileMap::iterator i = map_.begin(); i != map_.end(); ++i)
	{
		Color c(Color::WHITE);

		if(i->color() != Integrity::EMPTY && i->color() != Integrity::INVALID)
			c = Color::BLACK;

		cache_.add(CachedRange(*i, c));
	}

	for(Downloaders::const_iterator i = downloaders_.begin(); i != downloaders_.end(); ++i)
	{
		if((*i)->task().type == HttpTask::FETCH_FILE)
		{
			assert(!(*i)->task().range.empty());
			cache_.add(CachedRange((*i)->task().range, Color::BLACK));
		}
	}
}

bool RangeDestribute::need_sources() 
{
#warning ("optimize this")
	rebuild_cache();

	Cache::iterator i = cache_.begin();

	while(i != cache_.end() && i->color() == Color::BLACK) ++i;

	return i != cache_.end();
}

ByteRange RangeDestribute::find_range(Downloader::Ptr p) const
{
	for(Cache::iterator i = cache_.begin(); i != cache_.end(); ++i)
	{
		if(i->color() == Color::BLACK) continue;

		BOOST_FOREACH(const ByteRange& r, p->ranges())
		{
			if(r.intersects(*i)) 
			{
				const file_offset_t offset = std::max(r.offset(), i->offset());
				assert(std::min(r.last(), i->last()) >= offset);

				file_offset_t length = std::min(r.last(), i->last()) - offset + 1;
				length = std::min(file_offset_t(128*1024), length); //TODO fix it
				//length = std::min(max - offset % max, length);
			//	range.set(offset, length);
				return ByteRange(offset, length);
			}
		}
	}
	return ByteRange();
}

ByteRange RangeDestribute::find_random_range() const
{
	std::vector<ByteRange> empty_parts;
	//empty_parts.reserve(parts_.size());
	
//	pFileMap_->print();
	for(Cache::iterator i = cache_.begin(); i != cache_.end(); ++i)
		if(i->color() == Color::WHITE) 
			empty_parts.push_back(*i);
	
	if(empty_parts.empty()) 
		return ByteRange();
	
	ByteRange r = empty_parts[rand() % empty_parts.size()];

	const file_offset_t max_length = 128*1024;
	if(r.length() > max_length)
	{
		const file_offset_t diff = r.length() - max_length; 
		r.length(max_length);
		r.offset(r.offset() + rand() % diff);
	}

	return r;
}

ByteRange RangeDestribute::select_range(Downloader::Ptr p)
{
	if(cache_.empty() || cache_.size() == 1)
		rebuild_cache();

	ByteRange r = find_range(p);

	if(r.empty()) 
		r = find_random_range();

	if(!r.empty())
		cache_.add(CachedRange(r, Color::BLACK));

	return r;
}

} // namespace download_details

