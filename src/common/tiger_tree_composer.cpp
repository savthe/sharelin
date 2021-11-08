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

#include <cassert>

#include "tiger_tree_composer.hpp"
#include "hash/tiger.h"
#include "hash/ttr.h"
#include "hash/tiger.h"

namespace {

void tiger(const char* buf, std::size_t len, char* hash)
{
	::tiger((word64*)buf, len, (word64*)hash);
}

} //namespace 

void TigerTreeComposer::init(size_type sz, size_t d)
{
	TigerTreeBase::init(sz, d);
	tt_init(&context_);
}

void TigerTreeComposer::update(const char* buf, size_t len)
{
	while(len)
	{
		if(block_offset_ == block_size_)
			submit_block();

		const size_type left = block_size_ - block_offset_;
		const size_type n = std::min(left, static_cast<size_type>(len));

		tt_update(&context_, (unsigned char*)buf, n);
		block_offset_ += n;
		buf += n;
		len -= n;
	}
}

void TigerTreeComposer::submit_block()
{
	const size_t i = tree_.size() / 2 + block_count_;

	if(i >= tree_.size())
		throw std::range_error("Not enough blocks");

	TigerTreeNode& node = tree_[i];
	node.create();

	tt_digest(&context_, (unsigned char*)(node.begin()));
	tt_init(&context_);
	block_offset_ = 0;
	++block_count_;
}

void TigerTreeComposer::digest(TigerTree& tt_out)
{
	if(block_offset_) submit_block();

	const size_t cache_size = 1 + 2 * hash_size;
	char cache[cache_size];

	cache[0] = 1;

	size_t level_offset = tree_.size() / 2;
	while(level_offset && tree_[0].empty())
	{
		const size_t level_length = level_offset + 1;
		for(size_t i = level_offset; (i < level_offset + level_length) && !tree_[i].empty(); ++i)
		{
			TigerTreeNode& child = tree_[i];
			TigerTreeNode& parent = tree_[(i-1) / 2];
			if(parent.empty())
				parent = child;
			else
			{
				std::copy(parent.begin(), parent.end(), cache + 1);
				std::copy(child.begin(), child.end(), cache + 1 + hash_size);
				tiger(cache, cache_size, parent.begin());
			}
		}

		level_offset /= 2;
	}
	tt_out.copy_from(*this);
}



