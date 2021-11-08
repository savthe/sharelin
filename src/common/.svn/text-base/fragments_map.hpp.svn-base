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

#ifndef FRAGMENTS_MAP_HPP
#define FRAGMENTS_MAP_HPP

#include <map>
#include <stdexcept>
#include <cassert>

#include <boost/iterator/transform_iterator.hpp>

#include "fragments_map_util.hpp"
#include "map_value_iterator.hpp"

template <class F, class ColorComparer = FragmentsMapUtil::DefaultComparer<F> >
class FragmentsMap
{
public:
	typedef typename F::IndexType IndexType;
	typedef std::map<IndexType, F> Map;

private:
	struct Adaptor: public std::unary_function<const typename Map::value_type&, const F&>
	{
		const F& operator()(const typename Map::value_type& p) const
		{
			return p.second;
		}
	};	


public:
	typedef boost::transform_iterator<Adaptor, typename Map::const_iterator> iterator;
//	typedef map_value_const_iterator<IndexType, F> iterator;

	iterator begin() const { return boost::make_transform_iterator(map_.begin(), Adaptor()); }
	iterator end() const { return boost::make_transform_iterator(map_.end(), Adaptor()); }

	void add(const F& f)
	{
		using FragmentsMapUtil::first;
		using FragmentsMapUtil::last;

		if(f.length() == 0) 
			throw std::range_error("empty range");

		typename Map::iterator i = map_.find(f.offset());
		if(i != map_.end())
		{
			if(first(i->second) == first(f) && last(i->second) == last(f))
			{
				i->second = f;
				merge(i);
				return;
			}

			F tmp(i->second);
			map_.erase(i);
			if(tmp.length() > 1)
			{
				tmp.set_bound(first(tmp) + 1, last(tmp));
				insert(tmp);
			}
		}

		i = insert(f);
		fix_prev(i);
		fix_next(i);
		merge(i);
	}

	iterator find_holder(const IndexType& n) const
	{
		typename Map::iterator i = map_.upper_bound(n);
		if(i == map_.begin() || map_.empty())
			return end();

		--i;
		assert(n >= first(i->second) && n <= last(i->second));
		return iterator(i);
	}

	void clear() { map_.clear(); }
	bool empty() const { return map_.empty(); }
	std::size_t size() const { return map_.size(); }

private:

	inline typename Map::iterator insert(const F& f)
	{
		const std::pair<typename Map::iterator, bool> r = map_.insert(std::make_pair(f.offset(), f));
		assert(r.second);
		return r.first;
	}

	void merge(typename Map::iterator i)
	{
		using FragmentsMapUtil::first;
		using FragmentsMapUtil::last;

		ColorComparer comp;

		typename Map::iterator ileft(i);
		while(ileft != map_.begin() && comp(ileft->second, i->second))
			--ileft;

		if(!comp(ileft->second, i->second))
			++ileft;

		typename Map::iterator iright(i);
		while(iright != map_.end() && comp(i->second, iright->second))
			++iright;
		--iright;

		if(ileft == iright)
			return;

		F& f = ileft->second;
		f.set_bound(first(f), last(iright->second));
		++ileft;
		++iright;
		map_.erase(ileft, iright);
	}

	void fix_prev(typename Map::iterator i)
	{
		using FragmentsMapUtil::first;
		using FragmentsMapUtil::last;

		if(i == map_.begin()) return;

		const F& f(i->second);
		typename Map::iterator iprev(i);
		--iprev;
		F& prev(iprev->second);

		assert(first(prev) < first(f));

		if(last(prev) < first(f) - 1)
		{
			//hole
			F tmp;
			tmp.set_bound(last(prev) + 1, first(f) - 1);
			insert(tmp);
		}
		else if(last(prev) >= first(f))
		{
			if(last(prev) > last(f))
			{
				//split
				F tmp(prev);
				tmp.set_bound(last(f) + 1, last(prev));
				insert(tmp);
			}
			prev.set_bound(first(prev), first(f) - 1);
		}
	}

	void fix_next(typename Map::iterator i)
	{
		using FragmentsMapUtil::first;
		using FragmentsMapUtil::last;

		const F& f(i->second);

		typename Map::iterator inext = i;
		++inext;

		while(inext != map_.end() && last(f) >= last(inext->second))
		{
			typename Map::iterator itmp(inext);
			++inext;
			map_.erase(itmp);
		}

		if(inext != map_.end() && last(f) >= first(inext->second))
		{
			F r(inext->second);
			map_.erase(inext);
			r.set_bound(last(f) + 1, last(r));
			insert(r);
		}
	}

	Map map_;
};

#endif // FRAGMENTS_MAP_HPP

