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

#ifndef DISCOVERY_HPP
#define DISCOVERY_HPP

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>

#include "url.hpp"
#include "period.hpp"
#include "../httpant.hpp"
#include "io.hpp"

namespace g2 {

class Manager;

class Discovery
{
public:
	Discovery(Manager&);
	~Discovery();
	void start();
	void tick();
	void shutdown();
	void discover() { discover_ = true; }

private:
	struct WebCache
	{
		WebCache(): timestamp(0), banned(false) {}
		std::string address;
		time_t timestamp;
		bool banned;
	};

	typedef std::map<std::string, WebCache> Caches;

	enum {REST_TIME = 60};

	void Save();
	void LoadDefaults();
	void AddCache(const std::string& address);
	void AddCache(const WebCache& gwc);
	void HandleGwcReply();
	bool SelectGwc();
	void OnFinished(const HttpAnt::Error&);

	Manager& manager_;
	boost::shared_ptr<HttpAnt> pAnt_;
	Caches caches_;
	std::string address_;
	util::Period period_;
	bool discover_;
};

} // namespace g2

#endif //DISCOVERY_HPP
