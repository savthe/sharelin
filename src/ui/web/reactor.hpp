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

#ifndef WEBUIREACTOR_HPP
#define WEBUIREACTOR_HPP

#include "../common/default_reactor.hpp"

class UiMgr;

namespace ui {
namespace web {

class Reactor: public DefaultReactor
{
public:
	Reactor(UiMgr&);

protected:
	void ProcessQuery();
	uint PageSize() const;
	void QGroupAction() const;
	virtual void QUiset();
	virtual std::string SpaceString() const { return "&nbsp;"; }
};

} // namespace web
} // namespace ui

#endif //WEBUIREACTOR_HPP
