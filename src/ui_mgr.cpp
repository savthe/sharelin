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

#include "ui_mgr.hpp"
#include "verb.hpp"
#include "settings.hpp"

UiMgr::UiMgr():
	wui_(*this),
	tui_(*this)
{
}

void UiMgr::update_settings()
{
	wui_.UpdateSettings();
}

void UiMgr::start()
{
	const uint wui_port = app::settings().webui.port;
	if(wui_port)
	{
		verb1::app << "Starting web ui on port " << wui_port << std::endl;
		wui_.Start();
	}

	const uint tui_port = app::settings().telnet.port;
	if(tui_port) 
	{
		verb1::app << "Starting telnet on port " << tui_port << std::endl;
		tui_.Start();
	}
}

void UiMgr::shutdown()
{
	searches_.clear();
	wui_.stop();
	tui_.stop();
}

