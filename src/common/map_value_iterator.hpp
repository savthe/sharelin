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

#ifndef MAP_VALUE_ITERATOR
#define MAP_VALUE_ITERATOR

#include <map>

template <class Key, class Val>
class map_value_iterator
{
public:
	typedef typename std::map<Key, Val>::const_iterator map_iterator;
	map_value_iterator(const map_iterator& i): i_(i) {}
	map_value_iterator& operator++()
	{
		++i_;
		return *this;
	}

	Val& operator*() 
	{
		return i_->second;
	}

	const Val* operator->() 
	{
		return &i_->second;
	}
private:
	map_iterator i_;

	template <class K, class V>
	friend bool operator==(const map_value_iterator<K, V>&, const map_value_iterator<K, V>&);
};

template <class Key, class Val>
bool operator==(const map_value_iterator<Key, Val>& i1, const map_value_iterator<Key, Val>& i2)
{
	return i1.i_ == i2.i_;
}

template <class Key, class Val>
bool operator!=(const map_value_iterator<Key, Val>& i1, const map_value_iterator<Key, Val>& i2)
{
	return !(i1 == i2);
}

template <class Key, class Val>
class map_value_const_iterator
{
public:
	typedef typename std::map<Key, Val>::const_iterator map_iterator;
	map_value_const_iterator(const map_iterator& i): i_(i) {}
	map_value_const_iterator& operator++()
	{
		++i_;
		return *this;
	}

	const Val& operator*() const
	{
		return i_->second;
	}

	const Val* const operator->() const
	{
		return &i_->second;
	}
private:
	map_iterator i_;

	template <class K, class V>
	friend bool operator==(const map_value_const_iterator<K, V>&, const map_value_const_iterator<K, V>&);
};

template <class Key, class Val>
bool operator==(const map_value_const_iterator<Key, Val>& i1, const map_value_const_iterator<Key, Val>& i2)
{
	return i1.i_ == i2.i_;
}

template <class Key, class Val>
bool operator!=(const map_value_const_iterator<Key, Val>& i1, const map_value_const_iterator<Key, Val>& i2)
{
	return !(i1 == i2);
}

#endif // MAP_VALUE_ITERATOR

