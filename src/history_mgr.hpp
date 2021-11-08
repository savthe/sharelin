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

#ifndef HISTORYMGR_HPP
#define HISTORYMGR_HPP

#include <vector>

#include "inf/history.hpp"

class HistoryMgr
{
public:
	typedef std::vector<inf::history::Download> Downloads;
	typedef std::vector<inf::history::Upload> Uploads;

	void Add(const inf::history::Download&);
	void Add(const inf::history::Upload&);


	void ClearDownloads() { downloads_.clear(); }
	void ClearUploads() { uploads_.clear(); }

	const Downloads& GetDownloads() const { return downloads_; }
	const Uploads& GetUploads() const { return uploads_; }

	void recent_version(const inf::history::RecentVersion& r) { recent_version_ = r; }
	const inf::history::RecentVersion recent_version() const { return recent_version_; }

private:
	Downloads downloads_;
	Uploads uploads_;
	inf::history::RecentVersion recent_version_;
};

#endif //HISTORYMGR_HPP



