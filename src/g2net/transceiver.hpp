/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009  Andrey Stroganov

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

#ifndef TRANSCEIVER_HPP
#define TRANSCEIVER_HPP

#include "types.hpp"
#include "departurecache.hpp"
#include "arrivalcache.hpp"
#include "io.hpp"
#include "../traf_client.hpp"

namespace g2 {

class Manager;

class Transceiver
{
public:
	Transceiver(Manager&);
	void Start(uint16 port);
	void shutdown();
	void send(ip::Endpoint, const g2::Packet&);

private:
	void OnTimer(const bs::error_code&);
	void Dispatch();
	void OnReceived(const bs::error_code& err, std::size_t bytesReceived);
	void OnSent(const bs::error_code& err, std::size_t bytesSent);
	void StartReceive();

	enum {BUFFER_SIZE = 1024};
	char receiveBuffer_[BUFFER_SIZE];

	char sendBuffer_[udp::MTU];

	Manager& manager_;

	ba::ip::udp::socket socket_;
	ba::deadline_timer timer_;

	DepartureCache departure_;
	ArrivalCache arrival_;
	bool sending_;
	ba::ip::udp::endpoint sender_;
	bool throttle_;
	TrafClient traf_in_;
	TrafClient traf_out_;
};

} // namespace g2

#endif //TRANSCEIVER_HPP
