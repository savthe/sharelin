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

#ifndef GATEKEEPER_HPP
#define GATEKEEPER_HPP

#include <string>

#include <boost/shared_ptr.hpp>

#include "io.hpp"
#include "ip.hpp"

class Gatekeeper
{
public:
	typedef boost::shared_ptr<Gatekeeper> Ptr;

	virtual void push(ip::Endpoint) = 0;
	virtual TcpSocket& socket() = 0;
	virtual ip::Endpoint endpoint() const = 0;
	virtual TcpSocketPtr releaseSocket() = 0;
	virtual void start() = 0;
	virtual void close() = 0; 
	virtual const std::string& message() const = 0;
	virtual ~Gatekeeper() {}
};

#endif //GATEKEEPER_HPP
