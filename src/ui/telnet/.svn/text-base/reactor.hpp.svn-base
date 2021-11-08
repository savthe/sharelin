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

#ifndef TELNETREACTOR_HPP
#define TELNETREACTOR_HPP

#include "../common/default_reactor.hpp"
#include "../common/query.hpp"

class UiMgr;

namespace ui{
namespace telnet{

class Reactor: public DefaultReactor
{
public:
	Reactor(UiMgr&);

protected:
	void ProcessQuery();
	Query lastQuery_;
	virtual uint PageSize() const;
	virtual std::string SpaceString() const { return " "; }
};

} // namespace telnet
} // namespace ui

#endif //TELNETREACTOR_HPP
