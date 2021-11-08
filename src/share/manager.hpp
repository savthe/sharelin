/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008  Andrey Stroganov <savthe@gmail.com>

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

#ifndef SHARE_MANAGER_HPP
#define SHARE_MANAGER_HPP

#include <ctime>

#include <boost/noncopyable.hpp>

#include "types.hpp"
#include "period.hpp"

#include "localqht.hpp"
#include "hasher.hpp"
#include "storage.hpp"
#include "syncer.hpp"

namespace share_details {

class Storage;
class LocalQht;
class Hasher;

class Manager: public boost::noncopyable
{
public:
	Manager();
	~Manager();

	void start();
	void shutdown();
	void tick();
	void updateSettings();
	void sync();

	LocalQht& qht() 		{ return qht_; }
	Storage& storage() 	 	{ return storage_; }
	Hasher& hasher() 		{ return hasher_; }

private:
	bool load();
	bool save();
	void hash_something();
	void on_synced();

	LocalQht qht_;
	Storage storage_;
	Hasher hasher_;
	Syncer syncer_;

	util::Period periodSave_;
	util::Period period_sync_;
	std::time_t tsRehash_;
};

} //namespace share_details

#endif //SHARE_MANAGER_HPP

