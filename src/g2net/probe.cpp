/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011  Andrey Stroganov

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

#include "probe.hpp"
#include "manager.hpp"
#include "hubs.hpp"
#include "util.hpp"
#include "verb.hpp"
#include "settings.hpp"

using util::since;
using util::now;

namespace g2 {

Probe::Probe(Manager& manager):
	manager_(manager),
	detect_(false),
	firewalled_(true),
	probed_(0)
{
}

void Probe::start()
{
	using namespace settings_types::firewalled;

	detect_ = false;
	firewalled_ = true;

	switch(app::settings().net.firewalled)
	{       
		case YES: firewalled_ = true; break;
		case NO: firewalled_ = false; break;
		case AUTO: detect_ = true; break;
	};                          
}

void Probe::onPacketPO(const Packet& po, ip::Endpoint endpoint, ip::protocol::Type proto)
{
	if(!detect_) return;

	g2::Reader r(po);
	if(r.contains(g2::RELAY) && proto == ip::protocol::UDP)
		firewalled_ = false;
}

void Probe::probe(ip::Address addr)
{

	verb2::g2 << "Checking if we are firewalled via " << addr << std::endl;
	g2::Packet pi;
	g2::Writer w(pi);

	w.Begin(g2::PI);
	w.Begin(g2::UDP);
	w.Pod(manager_.selfEndpoint().address.value);
	w.Pod(manager_.selfEndpoint().port);
	w.Close(g2::UDP);
	w.Close(g2::PI);

	manager_.hubs().send(addr, pi);

	probed_ = now();
}

void Probe::tick()
{
	if(detect_ && firewalled_ && since(probed_) > PROBE_PERIOD)
	{
		const ip::Endpoint e = manager_.hubs().random();
		if(!e.empty()) probe(e.address);
	}

}

} // namespace g2

