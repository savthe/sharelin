/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov <savthe@gmail.com>

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

#ifndef UI_COMPARERS_HPP
#define UI_COMPARERS_HPP

#include <functional>

#include "ui/common/sortoption.hpp"
#include "inf/download.hpp"

namespace ui {
namespace Comparers {

struct Download: public std::binary_function<const inf::Download&, const inf::Download&, bool>
{
	Download(SortOption opt): opt_(opt) {}
	bool operator()(const inf::Download&, const inf::Download&) const;
	SortOption opt_;
};

struct SharedFiles: public std::binary_function<const FileInfo*, const FileInfo*, bool>
{
	SharedFiles(SortOption opt): opt_(opt) {}
	bool operator()(const FileInfo*, const FileInfo*) const;
	SortOption opt_;
};

} //namespace Comparers
} //namespace ui

#endif //UI_COMPARERS_HPP

