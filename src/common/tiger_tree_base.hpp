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

#ifndef TIGER_TREE_BASE_HPP
#define TIGER_TREE_BASE_HPP

#include <vector>

#include "tiger_tree_node.hpp"
#include "types.hpp"

class TigerTreeBase
{
protected:
	enum { hash_size = TigerTreeNode::hash_size };
	enum { tiger_block_size = 1024 };

	typedef file_offset_t size_type;

	TigerTreeBase();
	virtual ~TigerTreeBase() {}
	void init(size_type, size_t);
	void create_nodes();
	void clear();
	virtual void copy_from(const TigerTreeBase& other) { *this = other; }

	std::vector <TigerTreeNode> tree_;
	size_t total_blocks_;
	size_type block_size_;
};

#endif //TIGER_TREE_BASE_HPP

