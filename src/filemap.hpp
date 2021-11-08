/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010 Andrey Stroganov <savthe@gmail.com>

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

#ifndef FILEMAP_HPP
#define FILEMAP_HPP

#include "integrity.hpp"
#include "fragment.hpp"
#include <fragments_map.hpp>
#include "byterange.hpp"

class FileMap
{
private:
	typedef FragmentsMap<Fragment, FragmentColorComparer> Map;

public:
	typedef Map::iterator iterator;
	
	FileMap(): file_size_(0) {}
	FileMap(file_offset_t size) { init(size); }

	iterator begin() const { return map_.begin(); }
	iterator end() const { return map_.end(); }

	void init(file_offset_t size);
	
	void set_valid(const ByteRange& r) { modify(r, Integrity::VALID); }
	void set_full(const ByteRange& r) { modify(r, Integrity::FULL); }
	void set_invalid(const ByteRange& r) { modify(r, Integrity::INVALID); }
	void set_empty(const ByteRange& r) { modify(r, Integrity::EMPTY); }

	bool empty() const { return map_.empty(); }
	void clear() { map_.clear(); file_size_ = 0; }
	file_offset_t file_size() const { return file_size_; }

	Integrity status() const;
	Integrity status(const ByteRange& r) const;

	void print() const;

private:
	void modify(const ByteRange&, Integrity::Color);
	void debug_validate_change(const ByteRange& r, Integrity::Color to);

	Map map_;
	file_offset_t file_size_;
};

#endif //FILEMAP_HPP

