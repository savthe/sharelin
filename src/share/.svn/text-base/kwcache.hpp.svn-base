/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov <savthe@gmail.com>

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

#ifndef KWCACHE_HPP
#define KWCACHE_HPP

#include <map>
#include <set>
#include <list>
#include <string>
#include <functional>
#include <algorithm>
#include <cassert>

#include <boost/foreach.hpp>

template <class KeyType>
class KeywordCache
{
	typedef std::set<std::string> Keywords;
	typedef std::set<KeyType> Keys;
	typedef std::map<std::string, Keys> Cache;
	typedef std::map<KeyType, Keywords> Table;

	Cache cache_;
	Table table_;

	struct Contains: public std::unary_function<KeyType, bool>
	{
		Contains(const Table& table, const std::string& kw): table_(table), kw_(kw) {}

		bool operator()(const KeyType& test) const
		{
			typename Table::const_iterator ikws = table_.find(test);
			assert(ikws != table_.end());
			return ikws->second.find(kw_) != ikws->second.end();
		}

		const Table& table_;
		std::string kw_;
	};

	template<class C>
	struct SizeCompare
	{
		bool operator()(const C& e1, const C& e2)
		{
			return e1.size() < e2.size();
		}
	};

public:
	std::size_t size() const { return table_.size(); }
	
	template <typename T>
	void insert(const KeyType& key, T begin, T end)
	{
		remove(key);

		if (begin == end) return;

		typename Table::iterator i = table_.insert(std::make_pair(key, Keywords())).first;

		std::copy(begin, end, std::inserter(i->second, i->second.begin()));

		if(i->second.empty()) remove(key);
		else BOOST_FOREACH(const std::string& kw, i->second) cache_[kw].insert(key);
	}

	void remove(const KeyType& key)
	{
		typename Table::iterator i = table_.find(key);
		if(i == table_.end()) return;

		BOOST_FOREACH(const std::string& kw, i->second)
		{
			typename Cache::iterator ikw = cache_.find(kw);
			assert(ikw != cache_.end());

			ikw->second.erase(key);

			if(ikw->second.empty()) cache_.erase(ikw);
		}

		table_.erase(i);
	}

	template <typename I, typename E, typename T>
	void find(I iBegin, I iEnd, E eBegin, E eEnd, T out, std::size_t maxResults = 0) const
	{
		if (iBegin == iEnd) return;
		std::size_t sum = 0;
		for(Keywords::iterator i = iBegin; i != iEnd; ++i) sum += i->size();
		if (sum < 5) return;

		const I iMax = std::max_element(iBegin, iEnd, SizeCompare<std::string>());
		typename Cache::const_iterator ikeys = cache_.find(*iMax);
		if(ikeys == cache_.end()) return;

		std::list <KeyType> results;
		std::copy(ikeys->second.begin(), ikeys->second.end(), std::back_inserter(results));

		for(I i = iBegin; i != iEnd; ++i)
		{
			if (i == iMax) continue;
			results.erase(std::remove_if(results.begin(), results.end(), std::not1(Contains(table_, *i))), results.end());
		}

		for(E i = eBegin; i != eEnd; ++i)
			results.erase(std::remove_if(results.begin(), results.end(), Contains(table_, *i)), results.end());

		std::size_t n = 0;
		if (maxResults > 0 && maxResults < results.size()) n = results.size() - maxResults;
		while(n--) results.pop_back();
		
		std::copy(results.begin(), results.end(), out);
	}

};

#endif //KWCACHE_HPP

