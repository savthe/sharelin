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

#include "client.hpp"
#include "settings.hpp"
#include "io.hpp"
#include "reactor.hpp"
#include "server.hpp"
#include "settings.hpp"
#include "ui_mgr.hpp"

namespace ui {
namespace telnet {

Server::Server(UiMgr& m):
	base::Server(app::io(), new Reactor(m))
{
}

void Server::Start()
{
	Style(app::settings().telnet.style);
	base::Server::Start(app::settings().telnet.port);
}

std::string Server::StylePrefix() const
{
	return "telnet";
}

base::Server::ClientPtr Server::CreateClient()
{
	return boost::shared_ptr<Client>(new Client(this));
}

} // namespace telnet
} // namespace ui
