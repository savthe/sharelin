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

#ifndef SEARCH_KEYGATHER_HPP
#define SEARCH_KEYGATHER_HPP

#include <deque>
#include <list>

#include "ip.hpp"
#include "defs.hpp"
#include "period.hpp"
#include "../g2packet.hpp"

namespace search_details {

class KeyKeeper;

class KeyGather
{
	enum { MAX_ISSUE = 10 };

public:
	KeyGather(KeyKeeper&);

	void fresh(ip::Endpoint);
	void tick();
	void requestKeys() { needKeys_ = MAX_ISSUE; }
	void onQKA(const g2::Packet&, ip::Endpoint, ip::protocol::Type);

	void debug() const;

private:
	typedef std::map <ip::Address, QueriableHub> IssuedHubs;

	void query(ip::Endpoint);

	KeyKeeper& keeper_;
	IssuedHubs issued_;
	util::Period periodPurge_;

	unsigned int needKeys_;
};

} // namespace search_details

#endif // SEARCH_KEYGATHER_HPP

