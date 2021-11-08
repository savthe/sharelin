/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008  Andrey Stroganov <savthe@gmail.com>

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

#ifndef SHARE_LOCALQHT_HPP
#define SHARE_LOCALQHT_HPP

#include <ctime>

#include "queryhashtable.hpp"
#include "../fileinfo.hpp"

namespace share_details {

class LocalQht
{
public:
	LocalQht(): timestamp_(0) {}

	void update(const FileInfo&);
	void clear() { qht_.Reset(); }
	const QueryHashTable& get() const { return qht_; }
	std::time_t timestamp() const { return timestamp_; }

private:
	QueryHashTable qht_;
	std::time_t timestamp_;
};

} //namespace share_details

#endif //SHARE_LOCALQHT_HPP

