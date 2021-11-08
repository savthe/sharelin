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

#ifndef SHARE_STORAGE_HPP
#define SHARE_STORAGE_HPP

#include "kwcache.hpp"
#include "cache.hpp"
#include "idgen.hpp"
#include "stored_file.hpp"

namespace share_details {

class Storage 
{
public:
	StoredFile add(const FileInfo&);
	void replace(const FileInfo&);
	void remove(Id);
	void rename(Id, const std::string&);
	void finalize(Id);
	std::size_t size() const { return cache_.size(); }
	bool empty() const { return cache_.empty(); }

	template <typename Tag, typename Key> StoredFile find(Key key)
	{
		try { return cache_.template get<Tag>(key); }
		catch(std::exception&) {}
		return StoredFile();
	}

	template <typename Tag, typename Key> StoredFile get(Key key)
	{
		return cache_.get<Tag>(key); 
	}

	template <typename I, typename E, typename T> void findDn(I iBegin, I iEnd, E eBegin, E eEnd, T out, std::size_t maxResults = 0)
	{
		kwcache_.find(iBegin, iEnd, eBegin, eEnd, out, maxResults);
	}


	template <class T> void dump(T out) 
	{
		//Container v;
		//cache_.dump(std::back_inserter(v));
		//std::copy(v.begin(), v.end(), out);
		cache_.dump(out);
	}

private:
	void precache(StoredFile);
//	InfoPtr lock(WeakInfo);
	void recache(Id);

	Cache<StoredFile> cache_;
	KeywordCache<StoredFile> kwcache_;
	Misc::IdGenerator idgen_;
};
	
} //namespace storage

#endif //SHARE_STORAGE_HPP

