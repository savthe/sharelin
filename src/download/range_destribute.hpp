/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012  Andrey Stroganov <savthe@gmail.com>

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

#ifndef DL_RANGEDESTRIBUTE_HPP
#define DL_RANGEDESTRIBUTE_HPP

#include "fragments_map.hpp"
#include "byterange.hpp"
#include "colored_range.hpp"
#include "downloaders.hpp"
#include "../sharedfile.hpp"

namespace download_details {

class RangeDestribute
{
public:
	RangeDestribute(const FileMap& m, const Downloaders& d): map_(m), downloaders_(d) {}
	ByteRange select_range(Downloader::Ptr);
	bool need_sources(); 

private:
	void rebuild_cache();
	ByteRange find_range(Downloader::Ptr) const;
	ByteRange find_random_range() const;

	struct Color
	{
		enum color_t {WHITE, BLACK} value_;

		Color(): value_(WHITE) {}
		Color(color_t c): value_(c) {}

		bool operator==(const Color& rhs) const
		{
			return value_ == rhs.value_;
		}
	};

	typedef ColoredRange<file_offset_t, Color> CachedRange;
	typedef FragmentsMap<CachedRange, ColoredRangeComparer<file_offset_t, Color> > Cache;

	const FileMap& map_;
	const Downloaders& downloaders_;
	Cache cache_;
};

} // namespace download_details

#endif // DL_RANGEDESTRIBUTE_HPP

