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

#ifndef TICKER_CORE_HPP
#define TICKER_CORE_HPP

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class TickerCore: public boost::enable_shared_from_this<TickerCore>
{
public:
	typedef boost::function<void()> Handler;

	TickerCore(boost::asio::io_service&);

	void tick(boost::posix_time::time_duration, Handler);
	void cancel();

private:
	void on_tick(const boost::system::error_code&);

	boost::asio::deadline_timer timer_;
	Handler handler_;

};

#endif // TICKER_CORE_HPP

