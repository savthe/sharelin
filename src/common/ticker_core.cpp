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

#include <stdexcept>

#include <boost/bind.hpp>

#include "ticker_core.hpp"

TickerCore::TickerCore(boost::asio::io_service& io):
	timer_(io)
{
}

void TickerCore::tick(boost::posix_time::time_duration d, Handler h)
{
	if(handler_) throw std::logic_error("already ticking");

	handler_ = h;
	timer_.expires_from_now(d);
	timer_.async_wait(boost::bind(&TickerCore::on_tick, shared_from_this(), boost::asio::placeholders::error));
}

void TickerCore::on_tick(const boost::system::error_code& err)
{
	if(!handler_ || err) return;

	Handler h;
	h.swap(handler_);
	h();
}

void TickerCore::cancel()
{
	if(!handler_) return;

	Handler h;
	h.swap(handler_);
	timer_.cancel();
}
