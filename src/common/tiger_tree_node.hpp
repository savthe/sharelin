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

#ifndef TIGER_TREE_NODE_HPP
#define TIGER_TREE_NODE_HPP

#include <cassert>

class TigerTreeNode 
{
public:
	enum { hash_size = 24 };

	TigerTreeNode(): empty_(true) {}
	TigerTreeNode(const TigerTreeNode&);
	TigerTreeNode& operator=(const TigerTreeNode&);

	void create() 				{ empty_ = false; }

	const char* begin() const 	{ return bytes_; }
	char* begin() 				{ return bytes_; }
	const char* end() const 	{ return bytes_ + hash_size; }
	char* end() 				{ return bytes_ + hash_size; }
	bool empty() const 			{ return empty_; }

	void clear() { empty_ = true; } 

private:
	bool operator==(const TigerTreeNode&);

	char bytes_[hash_size];
	bool empty_;
};

#endif //TIGER_TREE_NODE_HPP

