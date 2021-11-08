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

#ifndef G2NET_NODE_HPP
#define G2NET_NODE_HPP

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "nodeinfo.hpp"
#include "ip.hpp"
#include "../g2packet.hpp"

namespace g2 {

class Node: private boost::noncopyable
{
public:
	typedef boost::shared_ptr<Node> Ptr;
	virtual ~Node() {} 

	virtual void close() = 0;
	virtual void send(const Packet& pk) = 0;
	virtual void tick() = 0;
	virtual void start() = 0;
	virtual NodeStats stats() const = 0;
	virtual NodeInfo info() const = 0;
};

} // namespace g2

#endif //G2NET_NODE_HPP
