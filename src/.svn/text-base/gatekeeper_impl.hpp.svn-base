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

#ifndef GATEKEEPERIMPL_HPP
#define GATEKEEPERIMPL_HPP

#include <vector>

#include <boost/enable_shared_from_this.hpp>

#include "gatekeeper.hpp"

class GateMgr;

class GatekeeperImpl: public Gatekeeper, public boost::enable_shared_from_this<GatekeeperImpl>
{
public:
	GatekeeperImpl(GateMgr*);
	~GatekeeperImpl();
	void push(ip::Endpoint);
	ba::ip::tcp::socket& socket() { return *pSock_; }
	ip::Endpoint endpoint() const { return endpoint_; }
	TcpSocketPtr releaseSocket() { return pSock_; }
	void start();
	void close(); 
	const std::string& message() const { return message_; }

private:
	void onData(const bs::error_code& err, std::size_t);
	void onConnected(const bs::error_code& err);
	void onGreetingSent(const bs::error_code& err);
	void onTimeout(const bs::error_code& err);
	void onSocketError(const bs::error_code&);
	void read();

	enum {BUFFER_SIZE = 16 * 1024};

	GateMgr* pGate_;
	TcpSocketPtr pSock_;
	ba::deadline_timer timer_;
	std::string message_;
	ip::Endpoint endpoint_;
	ba::streambuf buffer_;
	bool active_;
};

#endif //GATEKEEPERIMPL_HPP
