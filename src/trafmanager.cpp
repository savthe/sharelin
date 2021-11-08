/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov <savthe@gmail.com>

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

#include <algorithm>
#include <functional>
#include <boost/foreach.hpp>

#include "trafmanager.hpp"
#include "traf_client.hpp"

namespace {

struct PriorityComparer: public std::binary_function<const TrafClient*, const TrafClient*, bool>
{
	bool operator()(const TrafClient* c1, const TrafClient* c2) const
	{
		return c1->priority() > c2->priority();
	}
};

} // namespace 
TrafManager::TrafManager(std::size_t max_quota):
	max_quota_(max_quota),
	timestamp_(0),
	clients_dirty_(true)
{
}

void TrafManager::update(TrafClient*)
{
	clients_dirty_ = true;
}

TrafManager::~TrafManager()
{
	BOOST_FOREACH(TrafClient* c, clients_)
		c->manager_ = 0;
}

void TrafManager::attach(TrafClient* c)
{
	for(size_t i = 0; i < clients_.size(); ++i)
		assert(clients_[i] != c);

	clients_.push_back(c);
	clients_dirty_ = true;
	++moment_connections_.second;
}

void TrafManager::detach(TrafClient* c)
{
	for(Clients::iterator i = clients_.begin(); i != clients_.end(); ++i)
		if(*i == c)
		{
			clients_.erase(i);
			break;
		}

	clients_dirty_ = true;
}


void TrafManager::tick(std::time_t now)
{
	if(timestamp_ == now) return;
	timestamp_ = now;

	if(moment_connections_.first != now)
	{
		moment_connections_.first = now;
		moment_connections_.second = 0;
	}

	if(clients_dirty_)
	{
		std::sort(clients_.begin(), clients_.end(), PriorityComparer());
		clients_dirty_ = false;
	}

	const std::size_t minimal_reserved = 32;

	std::size_t quota = max_quota_;

	BOOST_FOREACH(TrafClient* c, clients_)
	{
		meter_.add(c->used_);

		std::size_t reserved = 0;

		if(quota)
		{
			const double mult = c->used_ == c->reserved_ ? 1.5 : 1.1;
			reserved = c->used_ * mult;
		}

		reserved = std::min(reserved, quota);
		reserved = std::max(reserved, minimal_reserved);
		quota -= std::min(quota, reserved); 

		c->used_ = 0;
		c->reserved_ = reserved;
		c->throttle_ = quota == 0;
	}

	if(quota)
	{
		const std::size_t q = quota / clients_.size();
		BOOST_FOREACH(TrafClient* c, clients_)
			c->reserved_ += q;
	}

	meter_.update();
}

