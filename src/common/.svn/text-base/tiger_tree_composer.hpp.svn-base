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

#ifndef TIGER_TREE_COMPOSER_HPP
#define TIGER_TREE_COMPOSER_HPP

#include <stdexcept>
#include <cassert>
	
#include "tiger_tree_base.hpp"
#include "hash/ttr.h"
#include "tigertree.hpp"

class TigerTreeComposer: private TigerTreeBase 
{
public:
	TigerTreeComposer(): block_offset_(0), block_count_(0) {}
	void init(size_type, size_t);
	void update(const char*, size_t);
	void digest(TigerTree&);
	const char* root() const { assert(!tree_.empty()); return tree_[0].begin(); }

private:
	void submit_block();

	TT_CONTEXT context_;
	size_type block_offset_;
	size_type block_count_;
};

#endif //TIGER_TREE_COMPOSER_HPP

