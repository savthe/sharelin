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

#include <boost/algorithm/string.hpp>

#include "gatereactor.hpp"
#include "verb.hpp"
#include "downloads.hpp"
#include "uploads.hpp"

using boost::istarts_with;

GateReactor& GateReactor::instance()
{
	static GateReactor obj;
	return obj;
}

GateReactor::Result GateReactor::react(Gatekeeper::Ptr p)
{
	const std::string& m = p->message();
	const ip::Endpoint ep = p->endpoint();
	const std::string::size_type npos = std::string::npos;
	
	Result r = INCOMPLETE;

	try
	{
		if(istarts_with(m, "GIV"))
		{
			if(m.size() >= 6 + 32) 
			{
				verb2::net << "Accepting GIV request from " << ep << std::endl;
				app::downloads().onGiv(p->releaseSocket(), ep, m);
				r = ACCEPTED;
			}
		}
		else if(istarts_with(m, "GET"))
		{
			if(m.find("\r\n\r\n") != npos) 
			{
				verb2::net << "Accepting GET request from " << ep << std::endl;
				app::uploads().onGet(p->releaseSocket(), ep, m);
				r = ACCEPTED;
			}
		}
		else r = REJECTED;
	}
	catch(std::exception& e)
	{
		verb2::net << "Couldn't accecpt connection from " << ep << " Error: " << e.what() << std::endl;
		r = REJECTED;
	}

	return r;
}

