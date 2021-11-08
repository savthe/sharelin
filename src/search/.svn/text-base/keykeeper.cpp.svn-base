/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011 Andrey Stroganov <savthe@gmail.com>

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

#include "keykeeper.hpp"
#include "tools.hpp"
#include "util.hpp"
#include "debug.hpp"

namespace search_details {

KeyKeeper::KeyKeeper():
	periodPurge_(boost::posix_time::minutes(10))
{
}

void KeyKeeper::rest(ip::Address addr, std::time_t until)
{
	const Hubs::iterator i = hubs_.find(addr);
	if (i != hubs_.end()) i->second.after = until;
}

void KeyKeeper::debug() const
{
#ifdef DEBUG
	std::cout << "Keeping keys: " << hubs_.size() << std::endl;
	for(Hubs::const_iterator i = hubs_.begin(); i != hubs_.end(); ++i)
	{
		const QueriableHub& h = i->second;
		std::cout << h.endpoint << " proxy: " << h.proxy << " issued: " << util::since(h.keyIssued) << " key: " << h.qkey << " ";
		if(util::now() > h.after) std::cout << " IDLE: " << util::since(h.after);
		else std::cout << " WAIT: " << h.after - util::now();
		std::cout << std::endl;
	}
#endif 
}

void KeyKeeper::unproxy(ip::Address proxy)
{
	erase_if(hubs_, HasProxy(proxy));
}

void KeyKeeper::tick()
{
	if(periodPurge_.set()) 
		erase_if(hubs_, OldKey(util::now(), KEY_TTL));
}

QueriableHub KeyKeeper::selectFor(SearcherImpl* p) const
{
	const std::time_t now = util::now();

	//TODO select random hub
	for(Hubs::const_iterator i = hubs_.begin(); i != hubs_.end(); ++i)
		if(now > i->second.after && !p->searched(i->second.endpoint.address)) return i->second;

	return QueriableHub();
}

} // namespace search_details

