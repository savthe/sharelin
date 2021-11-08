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

#ifndef G2NET_NODEIMPL_HPP
#define G2NET_NODEIMPL_HPP

#include <vector>
#include <queue>
#include <ctime>
#include <memory>

#include <boost/enable_shared_from_this.hpp>

#include "types.hpp"
#include "ip.hpp"
#include "period.hpp"
#include "nodeinfo.hpp"
#include "connector.hpp"
#include "g2sequencesender.hpp"
#include "g2packetreceiver.hpp"
#include "node.hpp"
#include "io.hpp"
#include "../g2packet.hpp"

namespace g2 { 

class Hubs; 

class NodeImpl: public Node, public boost::enable_shared_from_this<NodeImpl>
{
public:
	NodeImpl(Connector::Ptr, Hubs&);
	~NodeImpl();

	void close();
	void send(const g2::Packet& pk);
	void tick();
	void start();
	NodeStats stats() const { return stats_; }
	NodeInfo info() const { return myInfo_; }

private:
	void detachMe();
	void handlePacket(const Packet&);
	void onSent(const bs::error_code& err);
	void updateQht();
	void onPacket(const Packet&, const bs::error_code&);

	TcpSocketPtr pSock_;
	Hubs& hubs_;
	std::time_t timestamp_;
	std::time_t startTime_;
	util::Period periodLNI_;
	util::Period periodPing_;
	NodeInfo myInfo_;
	util::Period periodQHT_;
	std::time_t qhtTime_;
	NodeStats stats_;
	G2SequenceSender sender_;
	G2PacketReceiver receiver_;
};

} // namespace g2

#endif //G2NET_NODEIMPL_HPP
