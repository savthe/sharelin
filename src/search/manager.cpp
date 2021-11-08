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

#include <assert.h>

#include <boost/bind.hpp>

#include <io.hpp>

#include "manager.hpp"
#include "searcher_impl.hpp"

using boost::bind;

namespace search_details {

Manager::Manager():
	ticker_(app::io()), 
	gather_(keeper_),
	search_(keeper_, gather_)
{
}

void Manager::start()
{
	ticker_.tick(boost::posix_time::milliseconds(100), bind(&Manager::tick, this));
}

void Manager::shutdown()
{
	ticker_.cancel();
	search_.detach_all();
}

void Manager::onPacketQA(const g2::Packet& pk, ip::Endpoint endpoint, ip::protocol::Type proto)
{
	search_.onQA(pk, endpoint, proto);
}

void Manager::detach(SearcherImpl* s) 
{ 
	search_.detach(s);
}

void Manager::debug() const
{
	keeper_.debug();
	gather_.debug();
	search_.debug();
}

void Manager::onPacketQH2(const g2::Packet& qh2, ip::Endpoint endpoint, ip::protocol::Type proto)
{
	search_.onQH2(qh2, endpoint);
}

void Manager::onPacketQKA(const g2::Packet& qka, ip::Endpoint endpoint, ip::protocol::Type proto)
{
	gather_.onQKA(qka, endpoint, proto);
}

void Manager::tick()
{
	search_.tick();
	gather_.tick();
	keeper_.tick();
	ticker_.tick(boost::posix_time::milliseconds(100), boost::bind(&Manager::tick, this));
}

void Manager::attach(SearcherImpl* s)
{
	search_.attach(s);
}

} //namespace search_details

