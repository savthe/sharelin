/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012  Andrey Stroganov <savthe@gmail.com>

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

#ifndef SENDER_HPP
#define SENDER_HPP

#include <vector>
#include <cstddef>
#include <memory>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "bandwidth.hpp"

class Sender: private boost::noncopyable
{
public:
	typedef boost::function<void (const boost::system::error_code&)> Handler;

	template<typename T> Sender(boost::asio::ip::tcp::socket& s, T& bandwidth):
		socket_(s),
		timer_(s.get_io_service()),
		bandwidth_(new TBandwidth<T>(bandwidth)),
		chunk_size_(1024),
		cancel_(false),
		resting_(false)
	{ }

	void chunk_size(std::size_t s) { chunk_size_ = s; }

	template<typename T> void send(T& buffer, Handler handler)
	{
		cancel_ = false;
		buffer_.reset(new Buffer<T>(buffer));
		handler_ = handler;

		send();
	}

	void cancel()
	{
		if(!buffer_) return;

		cancel_ = true;

		if(resting_) timer_.cancel();
		else socket_.cancel();
	}

private:
	struct IBuffer
	{
		virtual void get(char*, std::size_t) = 0;
		virtual size_t remain() const = 0;
		virtual bool empty() const = 0;
		virtual ~IBuffer() {}
	};

	template<typename T> class Buffer: public IBuffer
	{
	public:
		Buffer(T& obj): obj_(obj) {}

		void get(char* v, std::size_t len) { obj_.get(v, len); }
		size_t remain() const { return obj_.remain(); }
		bool empty() const { return remain() == 0; }

	private:
		T& obj_;
	};

	template <class T> T min(const T& a, const T& b, const T& c)
	{
		return std::min(std::min(a, b), c);
	}

	void send()
	{
		if(bandwidth_->quota() == 0)
		{
			resting_ = true;
			timer_.expires_from_now(boost::posix_time::milliseconds(10));
			timer_.async_wait(boost::bind(&Sender::tick, this, boost::asio::placeholders::error));
			return;
		}

		const std::size_t size = min(bandwidth_->quota(), buffer_->remain(), chunk_size_);
		bandwidth_->consume(size);
		data_.resize(size);
		buffer_->get(&data_[0], size);

		boost::asio::async_write(socket_, boost::asio::buffer(data_), 
				boost::bind(&Sender::on_sent, this, boost::asio::placeholders::error));
	}

	void tick(const boost::system::error_code& err)
	{
		resting_ = false;

		if(err || cancel_) handle(err);
		else send();
	}

	void on_sent(const boost::system::error_code& err)
	{
		if(err || cancel_ || buffer_->empty()) handle(err);
		else send();
	}

	void handle(const boost::system::error_code& err)
	{
		buffer_.reset();

		Handler h;
		h.swap(handler_);
		h(err);
	}

	boost::asio::ip::tcp::socket& socket_;
	boost::asio::deadline_timer timer_;

	boost::scoped_ptr<IBandwidth> bandwidth_;
	boost::scoped_ptr<IBuffer> buffer_;

	Handler handler_;

	std::size_t chunk_size_;

	bool cancel_;
	bool resting_;

	std::vector<char> data_;

};

#endif // SENDER_HPP


