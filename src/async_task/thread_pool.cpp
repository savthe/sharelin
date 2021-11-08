/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012 Andrey Stroganov <savthe@gmail.com>

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

#include "thread_pool.hpp"
#include "verb.hpp"

using boost::bind;

namespace async_task {

ThreadPool::ThreadPool(boost::asio::io_service& io, TaskQueue& q, size_t sz):
	io_(io), queue_(q), size_(sz)
{
}

void ThreadPool::cancel_all()
{
	for(Jobs::iterator i = jobs_.begin(); i != jobs_.end(); ++i)
		cancel(*i);
}

void ThreadPool::cancel(Job* j)
{
	verb1::asy << "Cancelling async task: " << j->name() << " (ps: " << jobs_.size() << ")" << std::endl;
	j->cancel = true;
	j->thread.join();
	verb1::asy << "Task cancelled: " << j->name() << std::endl;
}

void ThreadPool::grab_tasks()
{
	while(jobs_.size() < size_ && !queue_.empty())
	{
		const Task t = queue_.front();
		queue_.pop_front();

		verb1::asy << "Running async task: " << t.name << std::endl;

		Job* j(new Job(t));
		jobs_.insert(j);
		j->thread = boost::thread(bind(&ThreadPool::async_run, this, j));
	}
}

void ThreadPool::cancel(TaskId id)
{
	Job* j = 0;
	for(Jobs::iterator i = jobs_.begin(); !j && i != jobs_.end(); ++i)
		if((*i)->task.id == id)
			j = *i;

	assert(j);

	cancel(j);
}

void ThreadPool::async_run(Job* j)
{
	j->task.cb_work();
	io_.post(bind(&ThreadPool::done, this, j));
}

void ThreadPool::done(Job* j)
{
	jobs_.erase(j);
	if(!j->cancel)
		j->task.cb_handler();

	delete j;

	grab_tasks();
}

} // namespace async_task

