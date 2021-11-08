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
#include <boost/foreach.hpp>

#include "server.hpp"
#include "verb.hpp"

using boost::bind;

namespace async_task {

Server::Server(boost::asio::io_service& io, std::size_t m):
	io_(io), 
	pool_(io, queue_, m)
{
}

void Server::attach(const Task& t)
{
	verb1::asy << "Attaching task: " << t.name << std::endl;

	queue_.push_back(t);

	pool_.grab_tasks();
}


void Server::cancel_all()
{
	pool_.cancel_all();

	queue_.clear();
}

void Server::cancel(TaskId id)
{
	for(TaskQueue::iterator i = queue_.begin(); i != queue_.end(); ++i)
		if(i->id == id)
		{
			queue_.erase(i);
			break;
		}

	pool_.cancel(id);
}

} // namespace async_task

