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

#include <stdexcept>

#include "traf_client.hpp"
#include "trafmanager.hpp"

TrafClient::TrafClient(TrafManager& m):
	manager_(&m),
	used_(0),
	reserved_(0),
	throttle_(false),
	priority_(0)
{
	manager_->attach(this);
}

TrafClient::~TrafClient()
{
	if(manager_)
		manager_->detach(this);
}

void TrafClient::priority(char p)
{
	priority_ = p;
	if(manager_)
		manager_->update(this);
}
