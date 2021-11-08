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

#ifndef ASYNC_WORK_HPP
#define ASYNC_WORK_HPP

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "async_task/defs.hpp"

class AsyncTaskServer;

class AsyncWork
{
public:
	typedef async_task::Callback Callback;

	AsyncWork(AsyncTaskServer& server): server_(server), id_(0) {}
	void run(Callback, Callback, const std::string&);
	void cancel();
	bool has_task() const { return id_; }

private:
	void done();

	AsyncTaskServer& server_;
	Callback handler_;
	async_task::TaskId id_;
};

#endif // ASYNC_WORK_HPP

