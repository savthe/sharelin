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

#ifndef NSASYNC_TASK_THREAD_POOL_HPP 
#define NSASYNC_TASK_THREAD_POOL_HPP

#include <set>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "task.hpp"
#include "defs.hpp"
#include "task_queue.hpp"

namespace async_task {

class ThreadPool
{
public:
	ThreadPool(boost::asio::io_service&, TaskQueue&, size_t);

	void cancel(TaskId);
	void cancel_all();
	void grab_tasks();

private:
	struct Job 
	{
		Job(const Task& t): task(t), cancel(false) {}

		const std::string name() const { return task.name; }
		Task task;
		bool cancel;
		boost::thread thread;
	};

	void cancel(Job*);
	void async_run(Job*);
	void done(Job*);

	boost::asio::io_service& io_;
	TaskQueue& queue_;
	size_t size_;

	typedef std::set<Job*> Jobs;
	Jobs jobs_;
};

} // namespace async_task

#endif // NSASYNC_TASK_THREAD_POOL_HPP
