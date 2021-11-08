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

#include "keymanager.hpp"
#include "neighbors.hpp"
#include "../app.hpp"
#include "../g2packet.hpp"
#include "../verb.hpp"
#include "../g2net.hpp"
#include "util.hpp"

namespace search_details {

KeyManager::KeyManager(Neighbors* p):
	pNeighbors_(p)
{
}

void KeyManager::tick()
{
	if (!needKeys_) return;

	ip::Endpoint ep;

	for (FreshHubs::const_iterator i = fresh_.begin(); ep.empty() && i != fresh_.end(); ++i)
		if (!keeper_.has(ep.address) && !pNeighbors_->has(ep.address)) ep = *i;

	if (!ep.empty()) query(ep);
}

void KeyManager::query(ip::Endpoint e)
{
	G2::Packet qkr;
	G2::Writer w(qkr);

	QueriableHub hub;
	hub.endpoint = e;

	hub.issued = util::now();

	if(app::firewalled())
	{
		hub.proxy = pNeighbors_->random();
		if(hub.proxy.empty()) return;

		w.Begin(G2::QKR);
		w.Begin(G2::QNA);
		w.Pod(hub.endpoint.address.value);
		w.Pod(hub.endpoint.port);
		w.Close(G2::QNA);
		w.Close(G2::QKR);

		verb3::sr << "Requesting query key from " << hub.endpoint << 
			" Using " << hub.proxy << " as proxy. Need keys: " << needKeys_ << std::endl;

		app::g2net()->sendTcp(hub.proxy.address, qkr);
	}
	else
	{
		const ip::Endpoint self = app::endpoint();
		assert(!self.empty());

		w.Begin(G2::QKR);
		w.Begin(G2::RNA);
		w.Pod(self.address.value);
		w.Pod(self.port);
		w.Close(G2::RNA);
		w.Close(G2::QKR);

		verb2::sr << "Requesting query key from " << hub.endpoint << 
			" Expecting direct reply. Want keys: " << needKeys_ << std::endl;

		app::g2net()->Send(qkr, hub.endpoint, ip::protocol::UDP);
	}

	issued_.push_back(hub);

//	hub.KeyIssued(app::now());
//	app::GetHubCache()->Update(hub);
}

} // namespace search_details

