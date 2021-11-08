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

#ifndef ASYNC_TASK_SERVER_HPP
#define ASYNC_TASK_SERVER_HPP

#include <boost/asio/io_service.hpp>

#include "async_task/task.hpp"

namespace async_task { class Server; }

class AsyncTaskServer
{
public:
	AsyncTaskServer(boost::asio::io_service& io, std::size_t m = 5);
	~AsyncTaskServer();

	void attach(const async_task::Task&);
	void cancel(async_task::TaskId);
	void cancel_all();

private:
	async_task::Server* impl_;
};


#endif // ASYNC_TASK_SERVER_HPP

