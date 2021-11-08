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

#include <boost/bind.hpp>

#include "gate.hpp"
#include "io.hpp"
#include "settings.hpp"
#include "verb.hpp"
#include "gatekeeper_impl.hpp"
#include "debug.hpp"

using boost::bind;

GateMgr::GateMgr():
	acceptor_(app::io(), ba::ip::tcp::v4())
{
}

GateMgr::~GateMgr()
{
	shutdown();
}

void GateMgr::debug()
{
#ifdef DEBUG
#endif
}

void GateMgr::start()
{
	const uint port = app::settings().net.listenPort;

	verb1::net << "Starting connection acceptor on port " << port << std::endl;

	ba::socket_base::reuse_address reuse(true);

	acceptor_.set_option(reuse);
	acceptor_.bind(ba::ip::tcp::endpoint(ba::ip::tcp::v4(), port));
	acceptor_.listen();

	startAccept();
}

void GateMgr::push(ip::Endpoint e)
{
	verb2::net << "Initiating GIV to " << e << std::endl;
	if(gatekeepers_.is_connected(e))
	{
		verb2::net << "Cancelling GIV (already connected) to " << e << std::endl;
		return;
	}
		
	Gatekeeper::Ptr p(new GatekeeperImpl(this));
	p->push(e);
	gatekeepers_.attach(e, p);
}

void GateMgr::detach(Gatekeeper::Ptr p)
{
	verb2::net << "Detaching gatekeeper " << p->endpoint() << std::endl;
	gatekeepers_.detach(p);
}

void GateMgr::startAccept()
{
	Gatekeeper::Ptr pGatekeeper(new GatekeeperImpl(this));
	verb2::net << "Waiting for new connection. Id: " << pGatekeeper.get() << std::endl;
	acceptor_.async_accept(pGatekeeper->socket(), boost::bind(&GateMgr::onAccept, this, pGatekeeper, ba::placeholders::error));
}

void GateMgr::shutdown()
{
	acceptor_.cancel();
	gatekeepers_.apply(bind(&Gatekeeper::close, _1));
	gatekeepers_.clear();
}

void GateMgr::onAccept(Gatekeeper::Ptr p, const bs::error_code& err)
{
	verb3::net << "Acception handler invoked with error " << err.message() << std::endl;
	if(err != ba::error::operation_aborted)
	{
		if(!err)
		{
			try
			{
				p->start();
				gatekeepers_.attach(p->endpoint(), p);
			}
			catch(std::exception& e)
			{
				verb2::net << "Couldn't accept connection: " << e.what() << std::endl;
			}
			//logger::bas << "Accepting incomming connection. Total gatekeepers: " << gatekeepers_.Count() << std::endl;
		}
		else verb2::net << "Error during accepting connection: " << err.message() << std::endl;
		startAccept();
	}
}

