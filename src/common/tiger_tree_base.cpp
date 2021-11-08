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

#include "tiger_tree_base.hpp"
#include <iostream>

TigerTreeBase::TigerTreeBase(): 
	total_blocks_(0),
	block_size_(0)
{
}

void TigerTreeBase::clear()
{
	tree_.clear();
	total_blocks_ = 0;
	block_size_ = 0;
}

void TigerTreeBase::init(size_type file_size, size_t max_depth)
{
	total_blocks_ = (file_size % tiger_block_size) ? file_size / tiger_block_size + 1 : file_size / tiger_block_size;
	
	size_t depth = 1;
	for(size_t i = 1; i < total_blocks_; i *= 2) 
		++depth; 
	
	block_size_ = tiger_block_size;
	while(max_depth && max_depth < depth) 
	{
		--depth;
		block_size_ *= 2;
		total_blocks_ = total_blocks_ % 2 ? (total_blocks_ + 1) / 2 : total_blocks_ / 2;
	}

	size_t nodes = 0;
	for(size_t i = 1, j = 1; i <= depth; ++i, j *= 2) nodes += j;
	
	assert(nodes);

	tree_.resize(nodes);
}

void TigerTreeBase::create_nodes()
{
	assert(tree_[0].empty());
	tree_[0].create();	
	for(size_t i = 0; i < total_blocks_; ++i)
	{
		size_t node = tree_.size() / 2 + i;
		while(node)
		{
			if(tree_[node].empty())
				tree_[node].create();

			node = (node - 1) / 2;
		}
	}
}

