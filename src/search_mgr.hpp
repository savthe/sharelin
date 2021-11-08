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

#ifndef SEARCH_MGR_HPP
#define SEARCH_MGR_HPP

#include <boost/noncopyable.hpp>

#include "ip.hpp"
#include "g2packet.hpp"
#include "searcher.hpp"

namespace search_details { class Manager; class SearcherImpl; }

class SearchMgr: public boost::noncopyable
{
public:
	SearchMgr();
	~SearchMgr();

	void start();
	void shutdown();

	void onPacketQKA(const g2::Packet& qka, ip::Endpoint endpoint, ip::protocol::Type proto);
	void onPacketQH2(const g2::Packet& qh2, ip::Endpoint endpoint, ip::protocol::Type proto);
	void onPacketQA(const g2::Packet& qa, ip::Endpoint endpoint, ip::protocol::Type proto);

private:
	friend class Searcher;
	void attach(search_details::SearcherImpl*);
	void detach(search_details::SearcherImpl*);

	search_details::Manager* impl_;
};

#endif //SEARCH_MGR_HPP

