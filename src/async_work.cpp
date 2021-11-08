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


#include "async_work.hpp"
#include "async_task_server.hpp"

using boost::bind;

void AsyncWork::cancel()
{
	if(id_)
	{
		server_.cancel(id_);
		handler_.clear();
		id_ = 0;
	}
}

void AsyncWork::done()
{
	id_ = 0;

	Callback h;
	h.swap(handler_);
	h();
}

void AsyncWork::run(Callback w, Callback h, const std::string& name)
{
	if(id_) throw std::logic_error("Work in progress");

	id_ = static_cast<async_task::TaskId>(this);
	handler_ = h;
	async_task::Task t(id_, w, bind(&AsyncWork::done, this), name);

	server_.attach(t);
}


