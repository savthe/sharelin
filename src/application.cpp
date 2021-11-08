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

#include "application.hpp"
#include "verb.hpp"
#include "filesystem.hpp"
#include "appdefs.hpp"
#include "util.hpp"
#include "downloads.hpp"
#include "io.hpp"
#include "settings_mgr.hpp"
#include "trafmanager.hpp"
#include "g2net_mgr.hpp"
#include "async_task_server.hpp"
#include "upload_mgr.hpp"
#include "history_mgr.hpp"
#include "security_mgr.hpp"
#include "gate_mgr.hpp"
#include "updateswatch.hpp"
#include "ticker.hpp"
#include "ui_mgr.hpp"
#include "share_mgr.hpp"
#include "search_mgr.hpp"

namespace {

const std::time_t HANDLE_LOG_PERIOD = 60;

template <typename T>
void startService(T& s, const std::string& name)
{
	verb1::app << "Starting " << name << std::endl;
	try
	{
		s->start();
	}
	catch(std::exception& e)
	{
		verb1::app << "Couldn't start " << name << " because of error: " << e.what() << std::endl;
		throw;
	}
}

} //namespace

using namespace boost;

Application& Application::inst()
{
	static Application a;
	return a;
}

Application::Application()
{
}

Application::~Application()
{
}

Guid Application::guid() const 
{
	return guid_;
}

void Application::update_settings()
{
	settings_mgr_->Save();
	traf_in_->quota(settings().net.maxBwIn * 0.95);
	traf_out_->quota(settings().net.maxBwOut * 0.95);
	ui_->update_settings();
	share_->updateSettings();
}

void Application::debug() const
{
}

void Application::create_modules()
{
	settings_mgr_.reset(new SettingsMgr);
	task_server_.reset(new AsyncTaskServer(app::io(), 5));
	g2net_.reset(new G2NetMgr);
	uploads_.reset(new UploadMgr);
	history_.reset(new HistoryMgr);
	security_.reset(new SecurityMgr);
	updates_watch_.reset(new UpdatesWatch(app::io()));
	gate_.reset(new GateMgr);
	ticker_.reset(new Ticker(app::io()));
	downloads_.reset(new DownloadMgr);
	share_.reset(new ShareMgr);
	searches_.reset(new SearchMgr);
	traf_in_.reset(new TrafManager);
	traf_out_.reset(new TrafManager);
	ui_.reset(new UiMgr);
}

void Application::init()
{
	verb1::app << "Starting " << app::name << std::endl;

	started_ = std::time(0);
	guid_.generate();

	create_modules();

	settings_mgr_->Start();

	FileSystem::PrepareFolder(settings().general.complete);
	FileSystem::PrepareFolder(settings().general.incomplete);
	FileSystem::PrepareFolder(settings().general.incomplete + "/inf");

	traf_in_->quota(settings().net.maxBwIn * 0.95);
	traf_out_->quota(settings().net.maxBwOut * 0.95);

	security_->Start();
	share_->start();
	searches_->start();

	if(settings().net.firewalled == settings_types::firewalled::YES)
		verb1::app << "Warning! Incomming connections are restricted. Sharelin will be less effective. See sharelin.conf" << std::endl;
	else gate_->start();

	g2net_->start();
	ui_->start();
	downloads_->Start();

	ticker_->tick(posix_time::milliseconds(100), bind(&Application::tick, this));
}

int Application::run()
{
	try { init(); }
	catch(std::exception& e)
	{
		verb1::app << "Couldn't start because of error: " << e.what() << std::endl;
		return 1;
	}
	
	verb1::app << "Running io" << std::endl;

	app::io().run();

	verb1::app << "Bye!" << std::endl;

	return 0;
}

void Application::tick()
{
	const std::time_t now = util::now();

	g2net_->tick();
	traf_in_->tick(now);
	traf_out_->tick(now);
	share_->tick();
	uploads_->OnTimer( now );
	updates_watch_->OnTimer();

	ticker_->tick(posix_time::milliseconds(100), bind(&Application::tick, this));
}

void Application::shutdown()
{
	app::io().post(boost::bind(&Application::stop, this));
}

DownloadMgr& Application::downloads() { return *downloads_; }
UploadMgr& Application::uploads() { return *uploads_; }
ShareMgr& Application::share() { return *share_; }
SearchMgr& Application::searches() { return *searches_; }
TrafManager& Application::traf_in() { return *traf_in_; }
TrafManager& Application::traf_out() { return *traf_out_; }
AsyncTaskServer& Application::task_server() { return *task_server_; }
const SettingsStorage& Application::settings() const { return settings_mgr_->storage(); }
SettingsMgr& Application::settings_mgr() { return *settings_mgr_; }
G2NetMgr& Application::g2net() { return *g2net_; }
SecurityMgr& Application::security() { return *security_; }
GateMgr& Application::gate() { return *gate_; }
HistoryMgr& Application::history() { return *history_; }

void Application::stop()
{
	verb1::app << "Shutting down" << std::endl;

	ticker_->cancel();

	verb1::app << "Stopping searches" << std::endl;
	searches_->shutdown();

	verb1::app << "Shutting down connections acceptor" << std::endl;
	gate_->shutdown();

	verb1::app << "Saving settings" << std::endl;
	settings_mgr_->Save();

	verb1::app << "Shutting down ui" << std::endl;
	ui_->shutdown();

	verb1::app << "Shutting down G2 network" << std::endl;
	g2net_->shutdown();

	verb1::app << "Shutting down download manager" << std::endl;
	downloads_->Shutdown();

	verb1::app << "Shutting down upload manager" << std::endl;
	uploads_->Shutdown();

	verb1::app << "Shutting down share manager" << std::endl;
	share_->shutdown();

	updates_watch_->Shutdown();

	verb1::app << "Aborting async tasks" << std::endl;
	task_server_->cancel_all();

	app::io().stop();
}

void Application::sync_share()
{
	share_->sync();
}

