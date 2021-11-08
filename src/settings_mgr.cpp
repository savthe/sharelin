/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011  Andrey Stroganov <savthe@gmail.com>

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

#include <iostream>
#include <string>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "settings_mgr.hpp"
#include "settings_defs.hpp"
#include "util.hpp"
#include "appdefs.hpp"
#include "env.hpp"

using namespace settings_details;
using namespace settings_types;

using boost::lexical_cast;
using boost::trim;
using boost::trim_copy;
using boost::iequals;
using boost::is_any_of;
using boost::split;
using boost::trim_right_if;
using util::split2;

void SettingsMgr::SetStorage(const SettingsStorage& s)
{
	storage_ = s;
}

void SettingsMgr::SetDefaultValues()
{
	storage_.general.userName = 		"User";
	storage_.general.incomplete = 	app::home() + "/incomplete";
	storage_.general.complete = 		app::home() + "/complete";

	storage_.net.queueSize =		10;
	storage_.net.maxHubs = 		2;
	storage_.net.maxBwIn = 		32 * 1024;
	storage_.net.maxBwOut = 		32 * 1024;
	storage_.net.listenPort = 		6349;
	storage_.net.listenIp = 		0;
	storage_.net.maxUploads = 		6;
	storage_.net.maxDownloads = 		50;
	storage_.net.maxFiles = 		50;
	storage_.net.pollMin =		40;
	storage_.net.pollMax = 		120;
	storage_.net.uploadsPerHost =		2;
	storage_.net.firewalled =		firewalled::AUTO;
	storage_.net.qkeyPeriod = 		60 * 60;

	storage_.webui.port =			0; //off if 0
	storage_.webui.style = 		"default";
	storage_.webui.pageSize = 		50;

	storage_.telnet.port =		0; //off if 0
	storage_.telnet.style = 		"default";
	storage_.telnet.pageSize = 		20;

	storage_.share.hashRate = 		0; //MBps
}


void SettingsMgr::Start()
{
	SetDefaultValues();

	try { Load(); }
	catch (std::exception& eload)
	{
		std::cout << "Couldn't load settings: " << eload.what()  << ". Using default values." << std::endl;
		try { Save(); }
		catch(std::exception& esave)
		{
			std::cout << "Couldn't save settings: " << esave.what() << std::endl;
			throw;
		}
	}
}

void SettingsMgr::Load()
{
	std::cout << "Loading settings from " << app::configFile() << std::endl;
	std::ifstream f(app::configFile().c_str());

	if(!f.good()) throw std::runtime_error("Couldn't open file");

	std::string line;
	int lineNumber = 0;
	while(std::getline(f, line))
	{
		++lineNumber;
		if(line.empty() || line[0] == '#') continue;

		std::string name;
		std::string value;
		try
		{
			split2(line, "=", name, value);
			SetOption(name, value);
		}
		catch (std::exception& e)
		{
			std::cout << "Bad option on line " << lineNumber << " What: " << e.what() << std::endl;
		}
	}
}

void SettingsMgr::Save()
{
	std::ofstream f(app::configFile().c_str());
	if(!f.good()) throw std::runtime_error("Couldn't open file");

	f << Options::userName << " = " << storage_.general.userName << std::endl;
	f << Options::listenPort << " = " << storage_.net.listenPort << std::endl;

	//firewalled
	f << Options::firewalled << " = ";
       	switch(storage_.net.firewalled)
	{
		case firewalled::YES: f << "yes"; break;
		case firewalled::NO: f << "no"; break;
		case firewalled::AUTO: f << "auto"; break;
	}
	f << std::endl;

	f << Options::complete << " = " << storage_.general.complete << std::endl;
	f << Options::incomplete << " = " << storage_.general.incomplete << std::endl;

	//share
	f << Options::share << " = ";
	BOOST_FOREACH(const std::string& folder, storage_.share.folders)
		f << folder << ";";
	f << std::endl;

	f << Options::maxHubs << " = " << storage_.net.maxHubs << std::endl;
	f << Options::maxBwIn << " = " << storage_.net.maxBwIn / 1024 << std::endl;
	f << Options::maxBwOut << " = " << storage_.net.maxBwOut / 1024 << std::endl;
	f << Options::maxFiles << " = " << storage_.net.maxFiles << std::endl;
	f << Options::maxDownloads << " = " << storage_.net.maxDownloads << std::endl;
	f << Options::maxUploads << " = " << storage_.net.maxUploads << std::endl;
	f << Options::hashRate << " = " << storage_.share.hashRate << std::endl;
	f << Options::webuiPort << " = " << storage_.webui.port << std::endl;
	f << Options::webuiStyle << " = " << storage_.webui.style << std::endl;
	f << Options::telnetPort << " = " << storage_.telnet.port << std::endl;
	f << Options::telnetStyle << " = " << storage_.telnet.style << std::endl;

	//remote ranges
	f << Options::remoteRanges << " = ";
	BOOST_FOREACH(IpSegment s, storage_.general.uisegments)
	{
		if(s.first() == s.last())
			f << s.first();
		else
			f << s.first() << "-" << s.last();
		f << "; ";
	}
	f << std::endl;

	f << Options::uploadsPerHost << " = " << storage_.net.uploadsPerHost << std::endl;
	f << Options::queueSize << " = " << storage_.net.queueSize << std::endl;
	f << Options::webuiPageSize << " = " << storage_.webui.pageSize << std::endl;
	f << Options::telnetPageSize << " = " << storage_.telnet.pageSize << std::endl;
}

void SettingsMgr::SetOptionFirewalled(const std::string& value)
{
	using namespace firewalled;

	if(iequals(value, "yes")) storage_.net.firewalled = YES;
	else if(iequals(value, "no")) storage_.net.firewalled = NO;
	else storage_.net.firewalled = AUTO;
}

void SettingsMgr::SetOptionComplete(const std::string& value)
{
	storage_.general.complete = value;
	if(storage_.general.complete.empty()) storage_.general.complete = app::home() + "/complete";
	if(storage_.general.complete[0] == '~')
		storage_.general.complete.replace(0, 1, env::home());
}

void SettingsMgr::SetOptionIncomplete(const std::string& value)
{
	storage_.general.incomplete = value;
	if(storage_.general.incomplete.empty()) storage_.general.incomplete = app::home() + "/incomplete";
	if(storage_.general.incomplete[0] == '~')
		storage_.general.incomplete.replace(0, 1, env::home());
}

void SettingsMgr::SetOptionShare(const std::string& value)
{
	storage_.share.folders.clear();
	std::vector<std::string> folders;
	split(folders, value, is_any_of(";"));
	BOOST_FOREACH(std::string f, folders)
	{
		trim(f);
		if(f.empty()) continue;
		if(f[0] == '~') f.replace(0, 1, env::home());
		trim_right_if(f, is_any_of("/"));
		if(!f.empty()) storage_.share.folders.push_back(f);
	}
}

void SettingsMgr::SetOptionRemoteRanges(const std::string& value)
{
	std::stringstream s(value);
	std::string strRange;
	storage_.general.uisegments.clear();
	while(std::getline(s, strRange, ';'))
	{
		trim(strRange);
		if(strRange.empty()) continue;

		ip::Address ip1, ip2;
		const std::size_t pos = strRange.find('-');

		if(pos == std::string::npos)
		{
			if(!ip1.fromString(strRange)) 
				throw std::runtime_error("Bad ip range: " + strRange);
			ip2 = ip1;
		}
		else
		{
			if(!ip1.fromString(strRange.substr(0, pos)) || !ip2.fromString(strRange.substr(pos + 1)))
				throw std::runtime_error("Bad ip range: " + strRange);
		}

		storage_.general.uisegments.push_back(IpSegment(ip1, ip2));
	}
}

void SettingsMgr::SetOption(const std::string& name, const std::string& value)
{
	if(iequals(name, Options::userName)) storage_.general.userName = value;
	else if(iequals(name, Options::maxHubs)) storage_.net.maxHubs = lexical_cast<uint>(value);
	else if(iequals(name, Options::maxBwIn)) storage_.net.maxBwIn = 1024 * lexical_cast<uint>(value);
	else if(iequals(name, Options::maxBwOut)) storage_.net.maxBwOut = 1024 * lexical_cast<uint>(value);
	else if(iequals(name, Options::listenPort)) storage_.net.listenPort = lexical_cast<uint>(value);
	else if(iequals(name, Options::queueSize)) storage_.net.queueSize = lexical_cast<uint>(value);
	else if(iequals(name, Options::maxUploads)) storage_.net.maxUploads = lexical_cast<uint>(value);
	else if(iequals(name, Options::maxDownloads)) storage_.net.maxDownloads = lexical_cast<uint>(value);
	else if(iequals(name, Options::maxFiles)) storage_.net.maxFiles = lexical_cast<uint>(value);
	else if(iequals(name, Options::uploadsPerHost)) storage_.net.uploadsPerHost = lexical_cast<uint>(value);
	else if(iequals(name, Options::telnetPort)) storage_.telnet.port = lexical_cast<uint>(value);
	else if(iequals(name, Options::webuiPort)) storage_.webui.port = lexical_cast<uint>(value);
	else if(iequals(name, Options::hashRate)) storage_.share.hashRate = lexical_cast<uint>(value);
	else if(iequals(name, Options::webuiStyle)) storage_.webui.style = value.empty() ? "default" : value;
	else if(iequals(name, Options::webuiPageSize)) storage_.webui.pageSize = lexical_cast<uint>(value);
	else if(iequals(name, Options::telnetStyle)) storage_.telnet.style = value.empty() ? "default" : value;
	else if(iequals(name, Options::telnetPageSize)) storage_.telnet.pageSize = lexical_cast<uint>(value);
	else if(iequals(name, Options::remoteRanges)) SetOptionRemoteRanges(value);
	else if(iequals(name, Options::share)) SetOptionShare(value);
	else if(iequals(name, Options::complete)) SetOptionComplete(value);
	else if(iequals(name, Options::incomplete)) SetOptionIncomplete(value);
	else if(iequals(name, Options::firewalled)) SetOptionFirewalled(value);
	else throw std::runtime_error("Unknown option: " + name); 
}

