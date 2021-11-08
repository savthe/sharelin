/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009  Andrey Stroganov <savthe@gmail.com>

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

#include "app.hpp"
#include "application.hpp"
#include "g2net.hpp"

namespace {

Application& inst()
{
	return Application::inst();
}

} // namespace

namespace app
{
	void debug()	{ inst().debug(); }
	void shutdown() { return inst().shutdown(); }
	void sync_share() { inst().sync_share(); }

	Guid guid() { return inst().guid(); }
	std::time_t started() { return inst().started(); }
	ip::Endpoint endpoint() { return g2net().selfEndpoint(); }
	bool firewalled() { return g2net().firewalled(); }
}
