/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009  Andrey Stroganov <savthe@gmail.com>

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

#ifndef DL_MANAGER_HPP
#define DL_MANAGER_HPP

#include <ctime>
#include <vector>

#include "types.hpp"
#include "idgen.hpp"
#include "download_ptr.hpp"
#include "io.hpp"
#include "ip.hpp"
#include "../inf/download.hpp"
#include "download_recipe.hpp"

namespace download_details {

class Manager
{
public:
	Manager();
	Id create(const DownloadRecipe&);
	void Start();
	void Shutdown();
	void Detach(DownloadPtr);
	void onGiv(TcpSocketPtr pSock, ip::Endpoint endpoint, const std::string& message);
	void set_priority(Id, uint);
	void pause(Id);
	void resume(Id);
	void cancel(Id);
	inf::Download info_download(Id) const ;
	inf::DownloadManager info() const;

	/*
	template <typename T>
	void Dump(T out)
	{
		std::copy(downloads_.begin(), downloads_.end(), out);
	}
	*/

private:
	typedef std::vector<DownloadPtr> Downloads;
	DownloadPtr get(Id) const;
	void update_queue();

	Downloads downloads_;
	Misc::IdGenerator idgen_;
};

} // namespace download_details

#endif //DL_MANAGER_HPP

