/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011  Andrey Stroganov <savthe@gmail.com>

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

#ifndef G2SEQUENCESENDER_HPP
#define G2SEQUENCESENDER_HPP

#include <queue>
#include <cstddef>

#include "g2packetsender.hpp"

class G2SequenceSender
{
public:
	typedef G2PacketSender::Handler Handler;

	G2SequenceSender(TcpSocket&);

	std::size_t size() const { return queue_.size(); }
	bool empty() const { return queue_.empty(); }
	bool sending() const { return sender_.sending(); }
	const g2::Packet& front() { return queue_.front(); }

	void push(const g2::Packet& pk) { queue_.push(pk); }
	void send(Handler);

private:
	G2PacketSender sender_;
	std::queue <g2::Packet> queue_;

};

#endif //G2SEQUENCESENDER_HPP

