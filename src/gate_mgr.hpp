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

#ifndef GATEMGR_HPP
#define GATEMGR_HPP

#include <set>
#include <memory>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "io.hpp"
#include "ip.hpp"
#include "connections.hpp"
#include "gatekeeper.hpp"
#include "guid.hpp"

class GateReactor;

class GateMgr: public boost::noncopyable
{
public:
	GateMgr();
	~GateMgr();

	void start();
	void detach(Gatekeeper::Ptr);
	void push(ip::Endpoint);
	void shutdown();
	void debug();

private:
	void startAccept();
	void onAccept(Gatekeeper::Ptr, const bs::error_code&);

	ba::ip::tcp::acceptor acceptor_;
	Connections<ip::Endpoint, Gatekeeper::Ptr> gatekeepers_;
};

#endif //GATEMGR_HPP

