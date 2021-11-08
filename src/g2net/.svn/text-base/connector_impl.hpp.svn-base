/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov

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

#ifndef G2NET_CONNECTORIMPL_HPP
#define G2NET_CONNECTORIMPL_HPP

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "connector.hpp"
#include "ip.hpp"

namespace g2 {

class ConnectorsManager;

class ConnectorImpl: public Connector, public boost::enable_shared_from_this<ConnectorImpl>
{
public:
	ConnectorImpl(ConnectorsManager&);
	~ConnectorImpl();
	void start(ip::Endpoint);
	TcpSocketPtr releaseSocket() { return pSocket_; }
	void close();
	g2::ConnectorInfo info() const { return info_; }
	ip::Endpoint endpoint() const { return info_.endpoint; }

private:
	void onConnected(const bs::error_code& err);
	void onGreetingSent(const bs::error_code& err);
	void onGreetingResponse(const bs::error_code& err);
	void onG2Connected(const bs::error_code& err);
	void onTimeout(const bs::error_code& err);
	void onSocketError(const bs::error_code& err);
	void detachMe();
	void handleReply();
	void extractHubs(const std::string&);

	ConnectorsManager& connectors_;
	TcpSocketPtr pSocket_;
	ba::deadline_timer timer_;
	ba::streambuf request_;
	ba::streambuf reply_;
	bool closed_;
	ConnectorInfo info_;
};

} // namespace g2

#endif //G2NET_CONNECTORIMPL_HPP

