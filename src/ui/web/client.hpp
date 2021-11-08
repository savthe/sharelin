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

#ifndef WEBUICLIENT_HPP
#define WEBUICLIENT_HPP

#include <string>

#include <boost/enable_shared_from_this.hpp>

#include "io.hpp"
#include "ip.hpp"
#include "byterange.hpp"
#include "asyncfile.hpp"
#include "../common/client.hpp"


namespace ui { 
namespace web {

class Server;

class Client: public base::Client, public boost::enable_shared_from_this<Client>
{
public:
	Client(Server*);
	virtual ~Client();
	ba::ip::tcp::socket& Socket() { return socket_; }
	void Start();
	void stop();
	ip::Endpoint GetEndpoint();

private:
	enum Method {GET, POST};
	void OnHeadersReceived(const bs::error_code&);
	void OnContent(const bs::error_code&);
	void OnFileTransfer(const bs::error_code&);
	void OnPageHeaderSent(const bs::error_code&);
	void OnTransferCompleted(const bs::error_code&);
	void OnFinal(const bs::error_code&);
	void TranslateHeaders();
	void HandleRequest();
	void ServeFile();
	void ProcessQuery();
	void DetachMe();

	ba::ip::tcp::socket socket_;
	ba::streambuf message_;
	std::vector<char> buffer_;
	std::string content_;
	bool keepAlive_;
	std::string object_;
	file_offset_t offset_;
	ByteRange fileRange_;
	bool closed_;
	AsyncFile file_;
	Method method_;
	Server* pServer_;
};

} // namespace web
} // namespace ui

#endif //WEBUICLIENT_HPP
