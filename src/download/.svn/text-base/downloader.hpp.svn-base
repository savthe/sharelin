/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009  Andrey Stroganov <savthe@gmail.com>

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

#ifndef DL_DOWNLOADER_HPP
#define DL_DOWNLOADER_HPP

#include <ctime>
#include <vector>
#include <string>
#include <ostream>
#include <memory>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include "byterange.hpp"
#include "ip.hpp"
#include "io.hpp"
#include "../traf_client.hpp"
#include "../g2queueinfo.hpp"
#include "states.hpp"
#include "http_task.hpp"
#include "sources.hpp"
#include "inf/download.hpp"

class Receiver;

namespace download_details {

class Download;

class Downloader
{
public:
	typedef boost::function<void(const bs::error_code&)> Handler;
	typedef boost::shared_ptr<Downloader> Ptr;
	typedef std::vector<ByteRange> Ranges;

	Downloader(ip::Endpoint, Sources&);
	Downloader(TcpSocketPtr, ip::Endpoint, Sources&);

	~Downloader();

	void connect(Handler);
	void close();

	ip::Endpoint endpoint() 		const { return endpoint_; }
	const TrafClient& traf() 		const { return traf_; }
	const HttpTask& task()		const { return task_; }
	const g2::QueueInfo& queue() 	const { return queue_; }
	std::string tiger_uri() 		const { return tiger_uri_; }
	Ranges ranges() 				const { return ranges_; }
	bool closed()					const { return state_ == dlr_state::CLOSED; }
	dlr_state::state_t state()	const { return state_; }
	inf::Downloader info()			const;
	void priority(int8 n) { traf_.priority(n); }

	template <typename T> void fetch(const HttpTask& t, T& buf, Handler h)
	{
		if(buffer_.get()) throw std::logic_error("buffer is already used");
		buffer_.reset(new Buffer<T>(buf));
		task_ = t;
		handler_ = h;

		fetch();
	}

	static std::size_t instances() { return instances_; }

private:
	struct IBuffer
	{
		virtual void put(const char*, std::size_t) = 0;
		virtual void range(const ByteRange&) = 0;
	};

	template <typename T> class Buffer: public IBuffer
	{
	public:
		Buffer(T& obj): obj_(obj) {}

		void put(const char* s, std::size_t len)
		{
			obj_.put(s, len);
		}

		void range(const ByteRange& r)
		{
			obj_.range(r);
		}

	private:
		T& obj_;
	};

	void fetch();
	void done(const bs::error_code&);
	void headers_sent(const bs::error_code&);
	void process_request();
	void headers_received(const bs::error_code&);
	void compose_common_headers(std::ostream&);

	TcpSocketPtr socket_;
	ba::streambuf message_;
	ip::Endpoint endpoint_;
	TrafClient traf_;
	std::auto_ptr<Receiver> receiver_;

	HttpTask task_;
	Handler handler_;
	std::auto_ptr<IBuffer> buffer_;

	Ranges ranges_;
	g2::QueueInfo queue_;
	std::string tiger_uri_;
	bool pushed_;
	dlr_state::state_t state_;
	Sources& sources_;

	static std::size_t instances_;
};

} // namespace download_details

#endif //DL_DOWNLOADER_HPP

