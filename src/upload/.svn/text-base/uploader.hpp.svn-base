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

#ifndef UPLOAD_UPLOADER_HPP
#define UPLOAD_UPLOADER_HPP

#include <vector>
#include <string>

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

#include "io.hpp"
#include "types.hpp"
#include "sharedfile.hpp"
#include "g2queueinfo.hpp"
#include "g2httprequest.hpp"
#include "inf/upload.hpp"
#include "sender.hpp"
#include "traf_client.hpp"
#include "uploader_state.hpp"

class UploadBuffer;

namespace upload_details {

class Manager;

class Uploader: private boost::noncopyable, public boost::enable_shared_from_this<Uploader>
{
public:
	typedef boost::shared_ptr<Uploader> Ptr;

	Uploader(Manager&, TcpSocketPtr pSock, ip::Endpoint endpoint);
	~Uploader();

	void OnTimer( std::time_t );
	void Enqueue(const g2::QueueInfo& q) { qinfo_ = q; }
	void Close();
	void Start(const std::string& message);

	ip::Endpoint Address() const 	{ return endpoint_; }
	inf::Uploader info() const;

private:
	friend class UploaderStateFile;
	friend class UploaderStateTiger;

	enum StatusType{IDLE, QUEUED, TRANSFER, CLOSING, CLOSED};
	enum { RESERVED_BUFFER_SIZE = 256 * 1024 };

	class Unhandled: public std::runtime_error
	{
	public:
		Unhandled(int status, const std::string& msg): std::runtime_error(msg), status_(status) { }
		int Status() const { return status_; }
	private:
		int status_;
	}; 

	void DetachMe();
	void OnRequest( const bs::error_code& );
	void HandleRequest();
	void process_request(std::ostream&);
	void TranslateRequest();
	void OnResponseSent( const bs::error_code& );
	void ComposeCommonHeaders( std::ostream& );
	void on_data_sent( const bs::error_code& );
	void StartSession(std::ostream&);
	void setup_file();
	void process_queued(std::ostream&);
	Ptr ptr();

	Manager& manager_;
	TcpSocketPtr pSock_;
	ip::Endpoint endpoint_;
	TrafClient traf_;
	Sender sender_;

	ba::streambuf requestBuf_;
	ba::streambuf responseBuf_;
	std::vector< char > outData_;

	std::time_t timestamp_;
	StatusType status_;
	g2::QueueInfo qinfo_;
	g2::HttpRequest request_;
	SharedFile file_;
	boost::scoped_ptr<UploaderState> state_;
};

} // namespace upload_details

#endif //UPLOAD_UPLOADER_HPP

