/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012  Andrey Stroganov <savthe@gmail.com>

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

#ifndef TRAFMANAGER_HPP
#define TRAFMANAGER_HPP

#include <vector>
#include <ctime>
#include <cstddef>
#include <utility>

#include "trafmeter.hpp"
#include "types.hpp"

class TrafClient;

class TrafManager
{
public:
	TrafManager(std::size_t max_quota = 1024);
	~TrafManager();

	void tick(std::time_t);
	void quota(std::size_t max) { max_quota_ = max; }

	uint64 rate() const { return meter_.rate(); } 
	uint64 total() const { return meter_.total(); }
	std::size_t moment_connections() const { return moment_connections_.second; }

	void attach(TrafClient*);
	void detach(TrafClient*);
	void update(TrafClient*);

private:
	
	typedef std::vector<TrafClient*> Clients;

	Clients clients_;
	std::size_t max_quota_;
	std::time_t timestamp_;
	TrafMeter<uint64> meter_;
	std::pair<std::time_t, std::size_t> moment_connections_;
	bool clients_dirty_;
};

#endif //TRAFMANAGER_HPP

