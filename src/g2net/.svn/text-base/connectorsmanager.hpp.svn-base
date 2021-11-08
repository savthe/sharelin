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

#ifndef G2_CONNECTORSMANAGER_HPP
#define G2_CONNECTORSMANAGER_HPP

#include <cstddef>
#include <map>
#include <ctime>

#include "ip.hpp"
#include "connector.hpp"
#include "defs.hpp"
#include "nodesmanager.hpp"

namespace g2 {

class Manager;

class ConnectorsManager: public NodesManager<ip::Address, Connector::Ptr>
{
public:
	ConnectorsManager(Manager&);
	void tick();
	void discovered(ip::Endpoint, std::time_t);
	void onSelf(ip::Address);
	void onConnected(Connector::Ptr);
	void detach(Connector::Ptr);
	Endpoints dump() const;

private:
	void connect();

	Manager& manager_;
};

} // namespace g2

#endif //G2_CONNECTORSMANAGER_HPP

