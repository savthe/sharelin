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

#include <vector>
#include <fstream>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "manager.hpp"
#include "conv/conv.hpp"
#include "app.hpp"
#include "appdefs.hpp"
#include "verb.hpp"
#include "error.hpp"
#include "storage.hpp"
#include "localqht.hpp"
#include "hasher.hpp"
#include "util.hpp"
#include "tools.hpp"
#include "settings.hpp"
#include "debug.hpp"

using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::iequals;
using boost::trim_copy;
using boost::istarts_with;
using boost::bind;

namespace share_details {

const std::time_t HASH_VALID_TIME = 7 * 24 * 3600;
const std::time_t REHASH_RUN_PERIOD = 3600;


} //namespace share_details

namespace share_details {

Manager::Manager():
	hasher_(storage_, qht_),
	syncer_(storage_),
	periodSave_(boost::posix_time::minutes(10)),
	period_sync_(boost::posix_time::minutes(30)),
	tsRehash_(0)
{
}

Manager::~Manager()
{
}

void Manager::start()
{
	verb1::shr << "Starting share manager" << std::endl;
	load();
	verb1::shr << "We have " << storage_.size() << " file(s) in share." << std::endl;
	if(storage_.empty())
		verb1::shr << "We should share something to support the G2 network." << std::endl; 
	hasher_.rate(app::settings().share.hashRate);
}

void Manager::sync()
{
	period_sync_.reset();
}

void Manager::updateSettings()
{
	hasher_.rate(app::settings().share.hashRate);
	tsRehash_ = 0;
}

void Manager::hash_something()
{
	assert(!hasher_.active());

	std::vector<StoredFile> v;
	storage_.dump(std::back_inserter(v));

	bool found = false;

	verb2::shr << "Looking for something to hash" << std::endl;
	const std::time_t now = util::now();
	BOOST_FOREACH (StoredFile f, v)
	{
		if(f.partial()) continue;

		if (now - f.hashed() > HASH_VALID_TIME)
		{
			verb1::shr << "Hashing file: " << f.path() << std::endl;
			hasher_.run(f.path());
			found = true;
			break;
		}
	}

	if (!found)
	{
		tsRehash_ = util::now();

		verb2::shr << "Rebuilding qht" << std::endl;
		qht_.clear();
		BOOST_FOREACH (StoredFile f, v)
			qht_.update(f.info());
	}

}

void Manager::on_synced()
{
	if(!hasher_.active())
		hash_something();
}

void Manager::tick()
{
	if (periodSave_.set()) save();

	if(!syncer_.active() && period_sync_.set())
		syncer_.sync(bind(&Manager::on_synced, this));


	if (util::since(tsRehash_) > REHASH_RUN_PERIOD && !hasher_.active()) hash_something();
}

void Manager::shutdown()
{
	hasher_.stop();
	syncer_.cancel();

	save();
}

bool Manager::load()
{
	const std::string fname = app::home() + "/share.dat";

	std::ifstream fin(fname.c_str());
	if(!fin) return false;

	while(fin)
	{
		FileInfo inf;
		try
		{
			if(!(fin >> inf)) break;
			
			if(inf.path.empty()) 
				throw std::runtime_error("Path is empty");

			inf.partial = boost::istarts_with(inf.path, app::settings().general.incomplete);

			storage_.add(inf);
			qht_.update(inf);
		}
		catch(std::exception& e)
		{
			verb1::shr << "Couldn't read file information: " << inf.path << ". Error: " << e.what() << std::endl;
		}
	}
	fin.close();

	return true;
}

bool Manager::save()
{
	const std::string fname = app::home() + "/share.dat";

	std::vector<StoredFile> v;
	storage_.dump(std::back_inserter(v));

	std::ofstream f(fname.c_str());
	if(!f) return false;

	for(uint i = 0; i < v.size(); ++i)
		f << v[i].info();

	f.close();

	return true;
}

} //namespace share_details

