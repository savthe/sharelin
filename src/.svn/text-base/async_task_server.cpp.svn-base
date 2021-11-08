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

#include <boost/bind.hpp>

#include "async_task_server.hpp"
#include "async_task/server.hpp"

using namespace async_task;

AsyncTaskServer::AsyncTaskServer(boost::asio::io_service& io, std::size_t m):
	impl_(new Server(io, m))
{ }

AsyncTaskServer::~AsyncTaskServer() { delete impl_; }
void AsyncTaskServer::attach(const Task& t) { impl_->attach(t); }
void AsyncTaskServer::cancel_all() { impl_->cancel_all(); }
void AsyncTaskServer::cancel(TaskId id) { impl_->cancel(id); }

