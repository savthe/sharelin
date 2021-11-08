/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011  Andrey Stroganov <savthe@gmail.com>

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

#ifndef CONNECTIONS_HPP
#define CONNECTIONS_HPP

#include <functional>
#include <map>
#include <stdexcept>
#include <cstddef>
#include "map_value_iterator.hpp"

#include <boost/iterator/transform_iterator.hpp>

struct AlreadyConnectedException: public std::logic_error
{
	AlreadyConnectedException(): std::logic_error("Already connected") {}
};

struct NotConnectedException: public std::logic_error
{
	NotConnectedException(): std::logic_error("Host is not connected") {}
};

template <typename Key, typename Ptr>
class Connections
{
public:
	typedef std::map<Key, Ptr> Container;

	struct IteratorAdaptor: public std::unary_function<const typename Container::value_type&, typename Ptr::element_type&>
	{
		typename Ptr::element_type& operator() (const typename Container::value_type& p) const
		{
			return *p.second;
		}
	};
	
	typedef boost::transform_iterator<IteratorAdaptor, typename Container::iterator> iterator;
	typedef boost::transform_iterator<IteratorAdaptor, typename Container::const_iterator> const_iterator;
//	typedef map_value_iterator<Key, Ptr> iterator;
//	typedef map_value_const_iterator<Key, Ptr> const_iterator;

	//typedef typename Container::iterator iterator;
	//typedef typename Container::const_iterator const_iterator;


	/*
	iterator begin() { return container_.begin(); }
	iterator end() { return container_.end(); }
	const_iterator begin() const { return container_.begin(); }
	const_iterator end() const { return container_.end(); }
	*/

	void attach(const Key& k, Ptr p)
	{
		bool found = false;

		for(typename Container::iterator i = container_.begin(); i != container_.end(); ++i)
			found |= i->first == k || i->second == p;

		if(found) throw AlreadyConnectedException();

		container_[k] = p;
	}

	void detach(const Key& k)
	{
		if(container_.find(k) == container_.end())
			throw NotConnectedException();
		container_.erase(k);
	}

	void detach(const Ptr& p)
	{
		for(typename Container::iterator i = container_.begin(); i != container_.end(); ++i)
			if(i->second == p)
			{
				container_.erase(i);
				return;
			}
		throw NotConnectedException();
	}

	Ptr get(const Key& k) const 
	{
		typename Container::const_iterator i = container_.find(k);
		if(i == container_.end())
			throw NotConnectedException();
		return i->second;
	}
	
	template <typename F> void apply(F f)
	{
		Container c(container_);
		for(typename Container::iterator i = c.begin(); i != c.end(); ++i)
			f(*i->second);
	}

	iterator begin() { return iterator(container_.begin()); }
	iterator end() { return iterator(container_.end()); }
	const_iterator begin() const { return const_iterator(container_.begin()); }
	const_iterator end() const { return const_iterator(container_.end()); }
	bool empty() const { return container_.empty(); }
	void clear() { container_.clear(); }
	std::size_t count() const { return container_.size(); }
	bool is_connected(Key key) const { return container_.find(key) != container_.end(); }

private:
	Container container_;
};

#endif // CONNECTIONS_HPP

