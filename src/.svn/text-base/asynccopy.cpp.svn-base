/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2010  Andrey Stroganov <savthe@gmail.com>

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

#include "asynccopy.hpp"
#include "filesystem.hpp"
#include "async_task_server.hpp"

using boost::bind;

AsyncCopy::AsyncCopy(AsyncTaskServer& s):
	work_(s)
{
}

void AsyncCopy::async_run()
{
	try 
	{ 
		FileSystem::CopyFile(from_, to_); 
	}
	catch ( boost::system::system_error& e )
	{
		error_ = e.code();
	}
}

void AsyncCopy::cancel()
{
	Handler h;
	h.swap(handler_);
	work_.cancel();
}

void AsyncCopy::done()
{
	Handler h;
	h.swap(handler_);
	h(error_);
}

void AsyncCopy::copy(const std::string& f, const std::string& t, Handler h)
{
	from_ = f;
	to_ = t;
	handler_ = h;
	error_.clear();
	work_.run(bind(&AsyncCopy::async_run, this), bind(&AsyncCopy::done, this), "Copy file: " + f + " => " + t);
}

