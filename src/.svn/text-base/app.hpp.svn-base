/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009-2011  Andrey Stroganov <savthe@gmail.com>

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

#ifndef APP_HPP
#define APP_HPP

#include <ctime>

#include "ip.hpp"
#include "guid.hpp"

namespace app 
{
	void debug();
	void shutdown();
	void sync_share();
	std::time_t started();
	Guid guid();
	ip::Endpoint endpoint();
	bool firewalled();

} //namespace app

#endif //APP_HPP

