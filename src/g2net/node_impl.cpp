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

#include <stdexcept>
#include <cassert>

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include "staticname.hpp"
#include "hubs.hpp"
#include "node_impl.hpp"
#include "../appdefs.hpp"
#include "../g2tools.hpp"
#include "../g2packet.hpp"
#include "../app.hpp"
#include "../share.hpp"
#include "../verb.hpp"
#include "util.hpp"

namespace g2 {

const std::time_t ALIVE_TIMEOUT = 20;

NodeImpl::NodeImpl(Connector::Ptr pConnector, Hubs& hubs):
	pSock_(pConnector->releaseSocket()),
	hubs_(hubs),
	timestamp_(0),
	periodLNI_(boost::posix_time::minutes(1)),
	periodPing_(boost::posix_time::seconds(15)),
	periodQHT_(boost::posix_time::minutes(5)),
	qhtTime_(0),
	sender_(*pSock_),
	receiver_(*pSock_)
{
	myInfo_.endpoint = pConnector->endpoint();
	myInfo_.agent = pConnector->info().agent;
}
	
NodeImpl::~NodeImpl()
{
}

void NodeImpl::send(const Packet& packet)
{
	sender_.push(packet);
	if (!sender_.sending()) 
		sender_.send(boost::bind(&NodeImpl::onSent, shared_from_this(), ba::placeholders::error));
}

void NodeImpl::onPacket(const Packet& pk, const bs::error_code& err)
{
	if (!pSock_->is_open()) return;

	if(!err)
	{
		try 
		{ 
			handlePacket(pk);
		}
		catch(std::exception& e)
		{
			verb2::g2 << "Host " << myInfo_.endpoint << " sent a corrupted packet: " << e.what() << std::endl;
			stats_.badPackets++;
		}

		receiver_.receive(boost::bind(&NodeImpl::onPacket, shared_from_this(), _1, _2));
	}
	else if(err != ba::error::operation_aborted) 
	{
		verb3::g2 << "G2NodeImpl::onPacket " << myInfo_.endpoint << " " << err.message() << std::endl;
		detachMe();
	}
}

void NodeImpl::onSent(const bs::error_code& err)
{
	if (!pSock_->is_open()) return;

	if(!err)
	{
		stats_.sentPackets++;
		if (!sender_.empty())
			sender_.send(boost::bind(&NodeImpl::onSent, shared_from_this(), ba::placeholders::error));
	}
	else if(err != ba::error::operation_aborted) 
	{
		verb3::g2 << "G2NodeImpl::onSent " << myInfo_.endpoint << " " << err.message() << std::endl;
		detachMe();
	}
}

void NodeImpl::start()
{
	timestamp_ = util::now();
	stats_.startTime = util::now();
	updateQht();
	receiver_.receive(boost::bind(&NodeImpl::onPacket, shared_from_this(), _1, _2));
}

void NodeImpl::handlePacket(const Packet& pk)
{
	Reader r(pk);
	if(r.type() == LNI)
	{
		while (r.next())
		{
			switch(r.type())
			{
				case GU:
				{
					Reader rc(r.child(), Reader::payload);
					rc.read(myInfo_.guid.begin(), 16);
					break;
				}
			}
		}

	}

	hubs_.onPacket(pk, myInfo_.endpoint);

	stats_.goodPackets++;
	timestamp_ = util::now();
}

void NodeImpl::updateQht()
{
	if(qhtTime_ == app::share().qhtTime()) return;
	qhtTime_ = std::time(0);

	verb2::g2 << "Updating QHT on " << myInfo_.endpoint << std::endl;

	const QueryHashTable& qht = app::share().qht();

	//sending reset
	Packet reset;
	composeQhtReset(reset, qht.SizeInBits());
	send(reset);

	//creating parts of QHT
	std::vector <Packet> qhtPackets;
	try
	{
		composeQht(app::share().qht(), 1024, std::back_inserter(qhtPackets));
	}
	catch (std::exception& e)
	{
		verb1::g2 << "Couldn't make qht packets: " << e.what() << std::endl;
	}

	//sending QHT
	BOOST_FOREACH (const Packet& pk, qhtPackets) send(pk);
}


void NodeImpl::tick()
{
	if(util::since(timestamp_) > ALIVE_TIMEOUT)
	{
		verb1::g2 << "Hub " << this << " " << myInfo_.endpoint << " timeout. Disconnecting." << std::endl;
		detachMe();
		return;
	}

	if (periodLNI_.set()) 
	{
		Packet lni;
		composeLni(lni, app::endpoint(), app::guid(), app::vid);
		send(lni);
	}

	if(periodQHT_.set()) updateQht();

	if(periodPing_.set()) 
	{
		Packet pk;
		composePi(pk);
		send(pk);
	}
}

void NodeImpl::close()
{
	if(pSock_->is_open()) pSock_->close();
}
	
void NodeImpl::detachMe()
{
	close();
	hubs_.detach(shared_from_this());
}

} // namespace g2
