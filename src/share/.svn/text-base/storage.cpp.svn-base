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

#include "storage.hpp"
#include "../querylang.hpp"
#include <boost/bind.hpp>

using boost::bind;

namespace share_details {

	/*
void testUpdate(const FileInfo& from, const FileInfo& to)
{
	typedef error::NotPermited NotPermited;

	if (from.path != to.path) throw NotPermited("Paths mismatch");
	if (from.size && from.size != to.size) throw NotPermited("Size mismatch");
	if (!from.sha1.empty() && from.sha1 != to.sha1) throw NotPermited("SHA1 mismatch");
	if (!from.ttr.empty() && from.ttr != to.ttr) throw NotPermited("TTR mismatch");
	if (!from.md5.empty() && from.md5 != to.md5) throw NotPermited("MD5 mismatch");
	if (!from.ed2k.empty() && from.ed2k != to.ed2k) throw NotPermited("ED2K mismatch");
	//if (!from.tigerTree.empty() && from.tigerTree != to.tigerTree) throw NotPermited("Tiger Tree mismatch";
	if (from.hits > to.hits) throw NotPermited("Hits decreased");
	if (from.requests > to.requests) throw NotPermited("Requests decreased");
}
*/

/*
void Storage::requested(WeakInfo m)
{
	lock(m)->requests++;
}

void Storage::hit(WeakInfo m)
{
	lock(m)->hits++;
}
*/

/*
InfoPtr Storage::lock(WeakInfo m)
{
	InfoPtr p = m.pInfo_.lock();
	if(!p) throw error::NotFound();
	return p;
}
*/

/*
void Storage::rename(WeakInfo w, const std::string& path)
{
	InfoPtr p = w.lock();
	p->path = path; 
	remove(p);
	add(p);
}
*/

/*
void Storage::finalize(WeakInfo w)
{
	w.lock()->partial = INVALID_ID;
}
*/
	
void Storage::precache(StoredFile f)
{
	cache_.insert(f);

	std::set <std::string> keywords;
	G2::ExtractKeywords(f.info().title(), std::inserter(keywords, keywords.begin()));
	kwcache_.insert(f, keywords.begin(), keywords.end());
}

void Storage::remove(Id id)
{
	StoredFile f(cache_.get<tag::id>(id));
	cache_.remove(f);
	kwcache_.remove(f);
}

StoredFile Storage::add(const FileInfo& info)
{
	FileInfo i(info);
	i.id = idgen_.get();
	StoredFile f(i, bind(&Storage::recache, this, _1));
	//InfoPtr p(new FileInfo(info));
	//p->id = idgen_.get();
	precache(f);
	return f;
}

void Storage::recache(Id id)
{
	StoredFile f(cache_.get<tag::id>(id));
	cache_.remove(f);
	kwcache_.remove(f);
	precache(f);
	/*
	cache_.update(f);

	InfoPtr p = cache_.get<tag::id>(f.id);
	testUpdate(*p, f);
	*p = f;
	cache_.update(p);
	*/
}

/*
void Storage::replace(const FileInfo& f)
{
	InfoPtr p = cache_.get<tag::id>(f.id);
	*p = f;
	cache_.update(p);
}
*/

} //namespace share_details
