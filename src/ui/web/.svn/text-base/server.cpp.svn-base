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

#include "server.hpp"
#include "reactor.hpp"
#include "settings.hpp"
#include "client.hpp"
#include "debug.hpp"
#include "io.hpp"
#include "settings.hpp"
#include "ui_mgr.hpp"

namespace ui {
namespace web {

Server::Server(UiMgr& m):
	base::Server(app::io(), new Reactor(m))
{
//	pReactor_ = new WebuiReactor;
}

Server::~Server()
{
//	delete pReactor_;
}

void Server::debug() const
{
#ifdef DEBUG
#endif
}

void Server::Start()
{
	Style(app::settings().webui.style);
	base::Server::Start(app::settings().webui.port);
}

void Server::UpdateSettings()
{
	Style(app::settings().webui.style);
}

base::Server::ClientPtr Server::CreateClient()
{
	return boost::shared_ptr<Client>(new Client(this));
}

std::string Server::StylePrefix() const
{
	return "webui";
}

/*
void WebuiServer::SetReloadRate(Ui::Tags::Type t, uint rate)
{
	reloadRates_[t] = rate;
}

uint WebuiServer::GetReloadRate(Ui::Tags::Type t) const
{
	if(reloadRates_.find(t) != reloadRates_.end())
		return reloadRates_.find(t)->second;
	else return 0;
}

void WebuiServer::ExpandDownloads(bool b)
{
	expandDownloads_ = b;
}

bool WebuiServer::ExpandDownloads() const
{
	return expandDownloads_;
}
*/

} // namespace web
} // namespace ui

