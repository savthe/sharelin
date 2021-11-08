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

#include <stdexcept>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include "syncer.hpp"
#include "settings.hpp"
#include "filesystem.hpp"
#include "share.hpp"
#include "verb.hpp"
#include "util.hpp"
#include "io.hpp"
#include "directorywalk.hpp"
#include "task_server.hpp"

using boost::bind;
using util::now;
using util::since;

namespace share_details {

Syncer::Syncer(Storage& s): 
	storage_(s), work_(app::task_server())
{
}

void Syncer::sync(Handler h)
{
	if(handler_) throw std::logic_error("Syncer is already active");

	verb2::shr << "Syncing shared files" << std::endl;
	handler_ = h;

	std::vector<std::string> v(app::settings().share.folders.begin(), app::settings().share.folders.end());
	v.push_back(app::settings().general.complete);

	walk_.assign(v.begin(), v.end(), FileSystem::predicate::FileSizeAtLeast(1));
	work_.run(bind(&DirectoryWalk::run, &walk_), bind(&Syncer::done, this), "Shared folders scan");
}

void Syncer::process_results()
{
	std::set<std::string> found(walk_.found_begin(), walk_.found_end());
	verb1::shr << "Found items: " << found.size() << std::endl;

	std::vector<SharedFile> v;
	app::share().dump(v);

	std::set<std::string> storing;

	BOOST_FOREACH(SharedFile f, v) 
	{
		if (!f->partial())
			storing.insert(f->path());
	}

	std::set <std::string> intersect;
	std::set <std::string> created;
	std::set <std::string> removed;

	std::set_intersection(found.begin(), found.end(), storing.begin(), storing.end(), std::inserter(intersect, intersect.begin()));
	std::set_symmetric_difference(intersect.begin(), intersect.end(), storing.begin(), storing.end(), std::inserter(removed, removed.begin()));
	std::set_symmetric_difference(intersect.begin(), intersect.end(), found.begin(), found.end(), std::inserter(created, created.begin()));

	BOOST_FOREACH (const std::string& s, removed) 
	{
		verb1::shr << "Removing file: " << s << std::endl;
		app::share().remove(s);
	}

	BOOST_FOREACH (const std::string& s, created) 
	{
		verb1::shr << "Adding file: " << s << std::endl;
		app::share().add(s);
	}

	verb1::shr << "Shared files synced" << std::endl;
}

void Syncer::done()
{
	process_results();

	Handler h;
	h.swap(handler_);
	h();
}

void Syncer::cancel()
{
	work_.cancel();
}

} // namespace share_details
