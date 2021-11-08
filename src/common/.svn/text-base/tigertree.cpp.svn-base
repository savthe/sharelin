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

#include "tigertree.hpp"
#include "hash/tiger.h"
#include "hash/ttr.h"

namespace {

void tiger(const char* buf, std::size_t len, char* hash)
{
	::tiger((word64*)buf, len, (word64*)hash);
}

} //namespace 

std::size_t TigerTree::depth() const
{
	size_t p = 1;
	size_t d = 0;
	while(p <= tree_.size())
	{
		p <<= 1;
		++d;
	}

	return d;
}

std::size_t TigerTree::size_in_bytes() const
{
	size_t sz = 0;
	for(size_t i = 0; i < tree_.size(); ++i)
		sz += tree_[i].empty() ? 0 : hash_size;
	return sz;
}

const char* TigerTree::get(TigerTree::size_type offset, TigerTree::size_type length) const
{
	if(empty()) throw std::range_error("Tree is empty");
	assert(block_size_);
	if(offset % block_size_) throw std::range_error("Block size is not a multiplier of offset");

	int64 parent = (tree_.size() / 2 + offset / block_size_);
	int64 len = length / block_size_ + (length % block_size_ ? 1 : 0);
	while(len > 1) 
	{
		parent /= 2;
		len = len % 2 ? len / 2 + 1 : len / 2;
	}
	
	return tree_[parent].begin();
	
}

bool TigerTree::test_range(file_offset_t offset, file_offset_t length, const char* p_hash) const
{
	if(empty()) throw std::range_error("Tree is empty");
	assert(block_size_);
	if(offset % block_size_) throw std::range_error("Block size is not a multiplier of offset");

	int64 parent = (tree_.size() / 2 + offset / block_size_);
	int64 len = length / block_size_ + (length % block_size_ ? 1 : 0);
	while(len > 1) {
		parent /= 2;
		len = len % 2 ? len / 2 + 1 : len / 2;
	}
	
	//return 0 == std::memcmp((char*)p_hash, (char*)tree_[parent]->begin(), 24);
	return std::equal(tree_[parent].begin(), tree_[parent].end(), p_hash);
}

void TigerTree::init(file_offset_t size, size_t depth)
{
	TigerTreeBase::init(size, depth);
	create_nodes();
}

bool TigerTree::empty() const
{
	return tree_.empty();
}

bool TigerTree::valid() const
{
	if(empty()) throw std::range_error("Tree is empty");

	bool valid = true;
	for(std::size_t parent = 0; valid && parent < tree_.size() / 2; ++parent)
	{
		if(tree_[parent].empty()) continue;

		const std::size_t left = parent * 2 + 1;
		const std::size_t right = parent * 2 + 2;

		if(left >= tree_.size() || tree_[left].empty())
			valid = false;
		else if(right >= tree_.size() || tree_[right].empty())
			valid = std::equal(tree_[parent].begin(), tree_[parent].end(), tree_[left].begin());
		else
		{
			assert(left < tree_.size() && right < tree_.size());
			char tmp[1 + 2 * hash_size];
			tmp[0] = 0x01;
			std::copy(tree_[left].begin(), tree_[left].end(), tmp + 1);
			std::copy(tree_[right].begin(), tree_[right].end(), tmp + 1 + hash_size);

			char hash[hash_size];
			tiger(tmp, 1 + 2 * hash_size, hash);
			valid = std::equal(tree_[parent].begin(), tree_[parent].end(), hash);
		}
	}

	return valid;
}

const char* TigerTree::root() const
{
	if(empty()) throw std::logic_error("Tree is empty");

	return tree_[0].begin();
}

/*int main()
{
	TigerTree tt;
	int f = open("Alash.jpg", O_RDONLY);
	int sz = lseek(f, 0, SEEK_END);
	std::cout << "file size: " << sz << std::endl;
	lseek(f, 0, SEEK_SET);
	tt.Create(sz, 4);
	int n;
	char buf[BLOCK_SIZE];
	int count = 0;
	//while(n = read(f, buf, BLOCK_SIZE)) {
	//	tt.AddBlock(buf, n, count);
//		count++;
//	}
	std::cout << "R: ";
//	PrintHex(tt.Root(), TIGER_SIZE, true);
//	file_offset_t x;
//	TigerTree tt;
//	do {
//		std::cin >> x;
//		tt.Create(x);
//		
//	}while(x);
	close(f);
	return 0;
}
*/
