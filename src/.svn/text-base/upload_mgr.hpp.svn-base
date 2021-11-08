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

#ifndef UPLOAD_MGR_HPP
#define UPLOAD_MGR_HPP

#include <ctime>

#include <boost/noncopyable.hpp>

#include "ip.hpp"
#include "inf/upload.hpp"
#include "io.hpp"

namespace upload_details { class Manager; }

class UploadMgr: private boost::noncopyable
{
public:
	UploadMgr();
	~UploadMgr();
	void OnTimer(std::time_t);
	void Accept(TcpSocketPtr, ip::Endpoint);
	void onGet(TcpSocketPtr pSock, ip::Endpoint endpoint, const std::string& message);
	void Shutdown();
	size_t count_uploads() const;
	inf::UploadManager info() const;

private:
	upload_details::Manager* impl_;
};

#endif //UPLOADM_MGR_HPP

