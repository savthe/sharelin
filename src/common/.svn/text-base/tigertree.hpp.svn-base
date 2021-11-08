/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008  Andrey Stroganov

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

#ifndef TIGERTREE_HPP
#define TIGERTREE_HPP

#include <stdexcept>
#include <iterator>
	
#include "tiger_tree_base.hpp"

class TigerTree: private TigerTreeBase 
{
public:
	struct Invalid: public std::runtime_error
	{
		Invalid(): std::runtime_error("Tree is invalid") {}
	};

	void copy_from(const TigerTreeBase& other) { TigerTreeBase::copy_from(other); }
	size_type block_size() const { return block_size_; }
	void init(size_type, size_t);
	const char* root() const;
	const char* get(size_type offset, size_type length) const;

	bool test_range(file_offset_t offset, file_offset_t length, const char* p_hash) const;
	bool empty() const;
	std::size_t size_in_bytes() const;

	template <typename T> void to_bytes(T out) const
	{
		for(size_t i = 0; i < tree_.size(); i++)
			if(!tree_[i].empty()) out = std::copy(tree_[i].begin(), tree_[i].end(), out);
	}

	size_t depth() const;

	template <typename T>
	void fromBytes(T begin, uint nbytes, size_type size)
	{
		if(nbytes % hash_size) throw Invalid();
	
		uint height = 1;
		uint nodes = 1;
		file_offset_t pwr = 1;
		while(nodes < nbytes / hash_size) 
		{
			height ++;
			pwr *= 2;
			nodes += pwr;
		}
		init(size, height);
	
		uint offset = 0, nd = 0;
		while(offset + hash_size <= nbytes && nd < nodes) 
		{
			if(!tree_[nd].empty()) 
			{
				T next = begin;
				std::advance(next, int(hash_size));
				std::copy(begin, next, tree_[nd].begin());
				begin = next;	
				offset += hash_size;
			}
			++nd;
		}

		if(!valid())
		{
			clear();
			throw Invalid();
		}
	}


private:
	bool valid() const;
};

#endif //TIGERTREE_HPP

