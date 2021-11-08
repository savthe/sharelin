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

#ifndef TRAFCLIENT_HPP
#define TRAFCLIENT_HPP

#include <cstddef>

#include "trafmeter.hpp"
#include "types.hpp"

class TrafManager;

class TrafClient
{
public:
	TrafClient(): manager_(0), used_(0), reserved_(0), throttle_(false), priority_(0) {}
	TrafClient(TrafManager&);
	~TrafClient();

	std::size_t quota() const { return reserved_ >= used_ ? reserved_ - used_ : 0; }
	
	bool throttle() const { return throttle_; }

	void consume(std::size_t n)
	{
		meter_.add(n);
		meter_.update();
		used_ += n;
	}

	uint64 total() const { return meter_.total(); }
	uint64 rate() const { return meter_.rate(); }
	std::time_t last_activity() const { return meter_.last_activity(); }
	char priority() const { return priority_; }
	void priority(char);

private:
	friend class TrafManager;

	TrafManager* manager_;

	std::size_t used_;
	std::size_t reserved_;
	bool throttle_;
	TrafMeter<uint64> meter_;
	char priority_;
};

#endif // TRAFCLIENT_HPP

