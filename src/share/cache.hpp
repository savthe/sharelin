/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2010 by Andrew Stroganov <savthe@gmail.com>

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

#ifndef SHARE_CACHE_HPP
#define SHARE_CACHE_HPP

#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include "hashes.hpp"
#include "types.hpp"
#include "error.hpp"
#include "tag.hpp"

namespace share_details {

template <typename T>
class Cache
{
private:
	struct CachedItem
	{
		CachedItem(const T& i):
			item(i),
			path(i.path()),
			sha1(i.sha1()),
			ttr(i.ttr()),
			ed2k(i.ed2k()),
			md5(i.md5()),
			id(i.id())
		{}

		T item;
		std::string path;
		hash::SHA1 sha1;
		hash::TTR ttr;
		hash::ED2K ed2k;
		hash::MD5 md5;
		Id id;
	};

	typedef boost::multi_index_container<
		CachedItem, boost::multi_index::indexed_by<
			boost::multi_index::ordered_unique< boost::multi_index::tag<tag::path>, BOOST_MULTI_INDEX_MEMBER(CachedItem, std::string, path)>,
			boost::multi_index::ordered_unique< boost::multi_index::tag<tag::id>, BOOST_MULTI_INDEX_MEMBER(CachedItem, Id, id)>,
			boost::multi_index::ordered_unique< boost::multi_index::tag<tag::st>, BOOST_MULTI_INDEX_MEMBER(CachedItem, T, item)>,
			boost::multi_index::ordered_non_unique< boost::multi_index::tag<tag::sha1>, BOOST_MULTI_INDEX_MEMBER(CachedItem, hash::SHA1, sha1)>,
			boost::multi_index::ordered_non_unique< boost::multi_index::tag<tag::md5>, BOOST_MULTI_INDEX_MEMBER(CachedItem, hash::MD5, md5)>,
			boost::multi_index::ordered_non_unique< boost::multi_index::tag<tag::ed2k>, BOOST_MULTI_INDEX_MEMBER(CachedItem, hash::ED2K, ed2k)>,
			boost::multi_index::ordered_non_unique< boost::multi_index::tag<tag::ttr>, BOOST_MULTI_INDEX_MEMBER(CachedItem, hash::TTR, ttr)>
		> 
	> Container;

public:
	void insert(const T& v)
	{
		if( !cache_.insert(CachedItem(v)).second ) 
			throw error::AlreadyShared();
	}

	void update(const T& v)
	{
		cache_.template get<tag::st>().replace(get_iterator<tag::st>(v), CachedItem(v));
	}

	void remove(const T& v)
	{
		cache_.template get<tag::st>().erase(v);
	}

	bool empty() const { return cache_.empty(); }
	std::size_t size() const { return cache_.size(); }

	template <typename R, typename Key>
	const T& get(Key key) const { return get_iterator<R>(key)->item; }

	template <typename I>
	void dump(I begin) const
	{
		for(typename Container::iterator i = cache_.begin(); i != cache_.end(); ++i)
			*begin++ = i->item;
	}

private:

	template <typename Tag, typename Key> 
	typename Container::template index_iterator<Tag>::type get_iterator(const Key& key) const
	{
		typename Container::template index_iterator<Tag>::type i = cache_.template get<Tag>().find(key);
		if(i != cache_.template get<Tag>().end()) 
			return i;
		throw error::NotFound();
	}

	Container cache_;
};

} //namespace share_details

#endif //SHARE_CACHE_HPP
