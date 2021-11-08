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

#ifndef SEARCH_MANAGER_HPP
#define SEARCH_MANAGER_HPP

#include <memory>

#include <boost/noncopyable.hpp>

#include "ip.hpp"
#include "../g2packet.hpp"
#include "ticker.hpp"
#include "searcher_impl.hpp"
#include "keykeeper.hpp"
#include "keygather.hpp"
#include "search.hpp"

namespace search_details {

class Manager: public boost::noncopyable
{
public:
	Manager();

	void start();
	void shutdown();

	void attach(SearcherImpl* s);
	void detach(SearcherImpl* s); 

	void onPacketQKA(const g2::Packet& qka, ip::Endpoint endpoint, ip::protocol::Type proto);
	void onPacketQH2(const g2::Packet& qh2, ip::Endpoint endpoint, ip::protocol::Type proto);
	void onPacketQA(const g2::Packet& qa, ip::Endpoint endpoint, ip::protocol::Type proto);

	void debug() const;

private:
	void tick();

	Ticker ticker_;
	KeyKeeper keeper_;
	KeyGather gather_;
	Search search_;
};

} //namespace search_details

#endif //SEARCH_MANAGER_HPP

