/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011 Andrey Stroganov <savthe@gmail.com>

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

#ifndef SHARE_SYNCER_HPP
#define SHARE_SYNCER_HPP

#include <set>
#include <vector>
#include <string>
#include <ctime>

#include <boost/function.hpp>

#include "storage.hpp"
#include "async_work.hpp"
#include "directorywalk.hpp"

namespace share_details {

class Syncer
{
public:
	typedef boost::function<void()> Handler;

	Syncer(Storage&);

	void cancel();
	void sync(Handler);
	bool active() const { return work_.has_task(); }

private:
	void done();
	void process_results();

	Storage& storage_;
	AsyncWork work_;
	DirectoryWalk walk_;

	Handler handler_;
};

} // namespace share_details

#endif //SHARE_SYNCER_HPP

