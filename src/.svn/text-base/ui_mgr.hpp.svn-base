/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012 Andrey Stroganov <savthe@gmail.com>

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

#ifndef UI_MGR_HPP
#define UI_MGR_HPP

#include "ui/web/server.hpp"
#include "ui/telnet/server.hpp"
#include "ui/common/searches.hpp"

class UiMgr
{
public:
	UiMgr();
	void start();
	void shutdown();
	void update_settings();
	UiSearches& searches() { return searches_; }

private:
	ui::web::Server wui_;
	ui::telnet::Server tui_;
	UiSearches searches_;
};

#endif // UI_MGR_HPP


