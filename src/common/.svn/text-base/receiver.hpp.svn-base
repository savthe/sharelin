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

#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <vector>
#include <cstddef>
#include <memory>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "bandwidth.hpp"

class Receiver: private boost::noncopyable
{
public:
	typedef boost::function<void (const boost::system::error_code&)> Handler;

	template<typename T> Receiver(boost::asio::ip::tcp::socket& s, T& bandwidth):
		socket_(s),
		timer_(s.get_io_service()),
		bandwidth_(new TBandwidth<T>(bandwidth)),
		chunk_size_(1024),
		cancel_(false),
		resting_(false),
		offset_(0),
		length_(0)
	{ }

	void chunk_size(std::size_t s) { chunk_size_ = s; }

	template<typename T>
	void receive(std::size_t length, T& buffer, Handler handler)
	{
		offset_ = 0;
		cancel_ = false;
		length_ = length;
		buffer_.reset(new Buffer<T>(buffer));
		handler_ = handler;

		receive();
	}

	void cancel()
	{
		if(!buffer_.get()) return;

		cancel_ = true;

		if(resting_) timer_.cancel();
		else socket_.cancel();
	}

	void close()
	{
		if(!buffer_.get()) return;

		cancel_ = true;

		if(resting_) timer_.cancel();
		else socket_.close();
	}

private:
	struct IBuffer
	{
		virtual void put(const char*, std::size_t) = 0;
		virtual ~IBuffer() {}
	};

	template<typename T> class Buffer: public IBuffer
	{
	public:
		Buffer(T& obj): obj_(obj) {}

		void put(const char* v, std::size_t len)
		{
			obj_.put(v, len);
		}
	private:
		T& obj_;
	};

	template <class T> T min3(const T& a, const T& b, const T& c)
	{
		if(a <= b && a <= c) return a;
		if(b <= a && b <= c) return b;
		return c;
	}

	void receive()
	{
		if(bandwidth_->quota() == 0)
		{
			resting_ = true;
			timer_.expires_from_now(boost::posix_time::milliseconds(10));
			timer_.async_wait(boost::bind(&Receiver::tick, this, boost::asio::placeholders::error));
			return;
		}

		assert(length_ >= offset_);
		const std::size_t remain = length_ - offset_;
		const std::size_t size = min3(bandwidth_->quota(), remain, chunk_size_);
		bandwidth_->consume(size);
		data_.resize(size);

		boost::asio::async_read(socket_, boost::asio::buffer(data_), boost::asio::transfer_all(), 
				boost::bind(&Receiver::on_data, this, boost::asio::placeholders::error));
	}

	void tick(const boost::system::error_code& err)
	{
		resting_ = false;

		if(err || cancel_) handle(err);
		else receive();
	}

	void on_data(const boost::system::error_code& err)
	{
		if(!err && !cancel_) 
		{
			buffer_->put(&data_[0], data_.size());
			offset_ += data_.size();
		}

		if(err || cancel_ || offset_ == length_) handle(err);
		else
		{
			assert(!err && offset_ < length_);
			receive();
		}
	}

	void handle(const boost::system::error_code& err)
	{
		buffer_.reset();
		length_ = 0;
		offset_ = 0;

		Handler h;
		h.swap(handler_);
		h(err);
	}

	boost::asio::ip::tcp::socket& socket_;
	boost::asio::deadline_timer timer_;

	std::auto_ptr<IBandwidth> bandwidth_;
	std::auto_ptr<IBuffer> buffer_;

	Handler handler_;

	std::size_t chunk_size_;

	bool cancel_;
	bool resting_;
	std::size_t offset_;
	std::size_t length_;

	std::vector<char> data_;

};

#endif // RECEIVER_HPP


