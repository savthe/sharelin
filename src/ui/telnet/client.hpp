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

#ifndef TELNETCLIENT_HPP
#define TELNETCLIENT_HPP

#include <string>

#include "boost/enable_shared_from_this.hpp"

#include "io.hpp"
#include "ip.hpp"
#include "../common/client.hpp"

namespace ui {
namespace telnet {

class Server;

class Client: public base::Client, public boost::enable_shared_from_this<Client>
{
public:
	Client(Server*);
	ba::ip::tcp::socket& Socket() { return socket_; }
	void Start();
	void stop();
	ip::Endpoint GetEndpoint();
	virtual ~Client() {}

private:
	void OnReplySent(const bs::error_code&);
	void OnData(const bs::error_code&);
	void OnFinal(const bs::error_code&);
	void ProcessQuery(const std::string&);
	void DetachMe();
	std::string GetPromt() const;

	ba::ip::tcp::socket socket_;
	ba::streambuf message_;
	bool closed_;
	std::string reply_;
	Server* pServer_;
};

} // namespace telnet
} // namespace ui

#endif //TELNETCLIENT_HPP

