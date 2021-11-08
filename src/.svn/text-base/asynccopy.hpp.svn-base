/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov <savthe@gmail.com>

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

#ifndef ASYNCCOPY_HPP
#define ASYNCCOPY_HPP

#include <string>

#include <boost/function.hpp>
#include <boost/system/error_code.hpp>

#include "async_work.hpp"

class AsyncTaskServer;

class AsyncCopy
{
public:
	typedef boost::function< void(const boost::system::error_code&) > Handler;

	AsyncCopy(AsyncTaskServer&);

	void copy(const std::string&, const std::string&, Handler);
	std::string from() const { return from_; }
	std::string to() const { return to_; }
	void cancel();

private:
	void async_run();
	void done();

	AsyncWork work_;
	std::string from_;
	std::string to_;
	boost::system::error_code error_;
	Handler handler_;
};

#endif //ASYNCCOPY_HPP

