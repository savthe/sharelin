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

#include "download_mgr.hpp"
#include "download/manager.hpp"

DownloadMgr::DownloadMgr(): impl_(new download_details::Manager) { }
DownloadMgr::~DownloadMgr() { delete impl_; }

Id DownloadMgr::create(const DownloadRecipe& r) { return impl_-> create(r); }
void DownloadMgr::Start() { impl_->Start(); }
void DownloadMgr::Shutdown() { impl_->Shutdown(); }
void DownloadMgr::set_priority(Id i, uint p) { impl_->set_priority(i, p); }
void DownloadMgr::pause(Id id) { impl_->pause(id); }
void DownloadMgr::resume(Id id) { impl_->resume(id); }
void DownloadMgr::cancel(Id id) { impl_->cancel(id); }
inf::Download DownloadMgr::info_download(Id id) const { return impl_->info_download(id); }
inf::DownloadManager DownloadMgr::info() const { return impl_->info(); }
void DownloadMgr::onGiv(TcpSocketPtr s, ip::Endpoint e, const std::string& m) { impl_->onGiv(s, e, m); }

