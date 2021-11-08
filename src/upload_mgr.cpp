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

#include "upload_mgr.hpp"
#include "upload/manager.hpp"

UploadMgr::UploadMgr():
	impl_(new upload_details::Manager)
{
}

UploadMgr::~UploadMgr()
{
	delete impl_;
}

size_t UploadMgr::count_uploads() const
{
	return impl_->count_uploads();
}

void UploadMgr::Shutdown()
{
	impl_->Shutdown();
}

void UploadMgr::OnTimer( std::time_t now )
{
	impl_->OnTimer(now);
}

inf::UploadManager UploadMgr::info() const
{
	return impl_->info();
}

void UploadMgr::onGet(TcpSocketPtr pSock, ip::Endpoint endpoint, const std::string& message)
{
	impl_->onGet(pSock, endpoint, message);
}

