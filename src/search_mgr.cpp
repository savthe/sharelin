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

#include "search/manager.hpp"
#include "search/searcher_impl.hpp"
#include "search_mgr.hpp"

using namespace search_details;

SearchMgr::SearchMgr():
	impl_(new Manager)
{
}

SearchMgr::~SearchMgr()
{
	delete impl_;
}

void SearchMgr::start()
{
	impl_->start();
}

void SearchMgr::shutdown()
{
	impl_->shutdown();
}

void SearchMgr::attach(SearcherImpl* s)
{
	impl_->attach(s);
}

void SearchMgr::detach(SearcherImpl* s)
{
	impl_->detach(s);
}

void SearchMgr::onPacketQA(const g2::Packet& pk, ip::Endpoint endpoint, ip::protocol::Type proto)
{
	impl_->onPacketQA(pk, endpoint, proto);
}

void SearchMgr::onPacketQH2(const g2::Packet& qh2, ip::Endpoint endpoint, ip::protocol::Type proto)
{
	impl_->onPacketQH2(qh2, endpoint, proto);
}

void SearchMgr::onPacketQKA(const g2::Packet& qka, ip::Endpoint endpoint, ip::protocol::Type proto)
{
	impl_->onPacketQKA(qka, endpoint, proto);
}

