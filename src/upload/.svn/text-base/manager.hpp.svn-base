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

#ifndef UPLOAD_MANAGER_HPP
#define UPLOAD_MANAGER_HPP

#include <ctime>
#include <vector>

#include "io.hpp"
#include "ip.hpp"
#include "uploader.hpp"
#include "inf/upload.hpp"

namespace upload_details { 

class Manager
{
public:
	Manager();
	void OnTimer(std::time_t);
	void Accept(TcpSocketPtr, ip::Endpoint);
	void Detach(Uploader::Ptr);
	void onGet(TcpSocketPtr pSock, ip::Endpoint endpoint, const std::string& message);
	void Shutdown();
	void debug();
	size_t count_uploads() const { return uploaders_.size(); }
	inf::UploadManager info() const;

private:
	void UpdateQueue();

	std::vector< Uploader::Ptr > uploaders_;
};

} // namespace upload_details

#endif //UPLOAD_MANAGER_HPP

