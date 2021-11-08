/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009-2011  Andrey Stroganov <savthe@gmail.com>

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

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <ctime>

#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "guid.hpp"

class UiMgr;
class Ticker;
class GateMgr;
class G2NetMgr;
class ShareMgr;
class TrafLimit;
class Heartbeat;
class SearchMgr;
class UploadMgr;
class HistoryMgr;
class SecurityMgr;
class TrafManager;
class DownloadMgr;
class SettingsMgr;
class UpdatesWatch;
class ShareManager;
class AsyncTaskServer;
struct SettingsStorage;

class Application: public boost::noncopyable
{
public:
	~Application();
	static Application& inst();

	int run();
	void shutdown();
	void debug() const;
	void sync_share();

	Guid guid() const;
	std::time_t started() const { return started_; }

	void update_settings();

	DownloadMgr& downloads();
	UploadMgr& uploads();
	ShareMgr& share();
	G2NetMgr& g2net();
	SearchMgr& searches();
	TrafManager& traf_in();
	TrafManager& traf_out();
	AsyncTaskServer& task_server();
	const SettingsStorage& settings() const;
	SettingsMgr& settings_mgr();
	SecurityMgr& security();
	GateMgr& gate();
	HistoryMgr& history();

private:
	Application();

	void init();
	void stop();
	void tick();
	void create_modules();

	Guid guid_;
	std::time_t started_;

	boost::scoped_ptr <Ticker> ticker_;
	boost::scoped_ptr <DownloadMgr> downloads_;;
	boost::scoped_ptr <ShareMgr> share_;
	boost::scoped_ptr <TrafManager> traf_in_;
	boost::scoped_ptr <TrafManager> traf_out_;
	boost::scoped_ptr <SearchMgr> searches_;
	boost::scoped_ptr <UiMgr> ui_;
	boost::scoped_ptr <AsyncTaskServer> task_server_;;
	boost::scoped_ptr <SettingsMgr> settings_mgr_;
	boost::scoped_ptr <G2NetMgr> g2net_;
	boost::scoped_ptr <UploadMgr> uploads_;
	boost::scoped_ptr <SecurityMgr> security_;
	boost::scoped_ptr <GateMgr> gate_;
	boost::scoped_ptr <HistoryMgr> history_;
	boost::scoped_ptr <UpdatesWatch> updates_watch_;
};

#endif //APPLICATION_HPP

