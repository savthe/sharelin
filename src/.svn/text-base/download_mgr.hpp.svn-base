/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009-2012  Andrey Stroganov <savthe@gmail.com>

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

#ifndef DOWNLOAD_MGR
#define DOWNLOAD_MGR

#include <string>

#include "download_recipe.hpp"
#include "inf/download.hpp"
#include "types.hpp"
#include "ip.hpp"
#include "io.hpp"

namespace download_details { class Manager; }

class DownloadMgr
{
public:
	DownloadMgr();
	~DownloadMgr();
	Id create(const DownloadRecipe&);
	void Start();
	void Shutdown();
	void onGiv(TcpSocketPtr, ip::Endpoint, const std::string&);
	void set_priority(Id, uint);
	void pause(Id);
	void resume(Id);
	void cancel(Id);
	inf::Download info_download(Id) const ;
	inf::DownloadManager info() const;

private:
	download_details::Manager* impl_;
};

#endif // DOWNLOAD_MGR

