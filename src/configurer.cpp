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

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <map>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "settings_mgr.hpp"
#include "settings_defs.hpp"
#include "configurer.hpp"
#include "appdefs.hpp"
#include "util.hpp"

using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::trim_copy;
using boost::trim;
using boost::iequals;
using util::split2;

namespace Configurer
{
typedef std::vector< std::pair<std::string, std::string> > Options;
typedef std::map<std::string, std::string> Messages;

std::string Input(const std::string& message, const std::string& value)
{
	std::string s;
	std::cout << message << " [" << value << "]: ";
	std::getline(std::cin, s);
	return s.empty() ? value : s;
}

void LoadOptions(Options& opts)
{
	std::ifstream f(app::configFile().c_str());
	std::string line;

	if(!f) throw std::runtime_error("Couldn't open configuration file: " + app::configFile());

	while(std::getline(f, line))
	{
		trim(line);
		if(line.empty() || line[0] == '#') continue;
		try
		{
			std::string left;
			std::string right;
			split2(line, "=", left, right);
			opts.push_back(std::make_pair(left, right));
		}
		catch(std::exception&) { }
	}
}


void PopulateMessages(Messages& m)
{
	using namespace settings_details::Options;
	m[userName] = "User name";
	m[maxHubs]  = "Maximum number of hubs (2 or 3 is reasonable value)";
	m[maxBwIn] = "Maximum download rate in kilobytes per second";
	m[maxBwOut] = "Maximum upload rate in kilobytes per second";
	m[listenPort] = "Listen port";
	m[maxUploads] = "Maximum number of active uploads";
	m[maxDownloads] = "Maximum number of download connections";
	m[maxFiles] = "Maximum number of concurrent downloaded files";
	m[firewalled] = "Is this host firewalled and can not accept incomming connections? (yes, no, auto)";
	m[share] = "Shared folders separated by semicolons (example: /home/jey/music; /home/jey/video)";
	m[telnetPort] = "Telnet port (0 to disable telnet)";
	m[uploadsPerHost] = "Maximum number of uploads per host";
	m[webuiPort] = "Web interface port (0 to disable web user interface)";
	m[remoteRanges] = "Ip ranges of hosts who allowed to control Sharelin via telnet or web ui separated by semicolon(example 192.168.1.1; 10.16.32.5-10.16.32.10)";
	m[webuiStyle] = "Web interface style (preinstalled styles: default, dos)";
	m[telnetStyle] = "Telnet interface style (preinstalled styles: default)";
	m[complete] = "Folder to store complete files";
	m[incomplete] = "Folder to store incomplete files";
}

void Setup(Options& opts, SettingsMgr& mgr)
{
	Messages msgs;
	PopulateMessages(msgs);

	for(Options::iterator i = opts.begin(); i != opts.end(); ++i)
	{
		const std::string& key = i->first;
		const std::string& value = i->second;

		std::string m;

		for(Messages::iterator im = msgs.begin(); im != msgs.end(); ++im)
			if(iequals(im->first, key))
			{
				m = im->second;
				break;
			}

		if(m.empty()) continue;

		for(;;)
		{
			try
			{
				mgr.SetOption(key, Input(m, value));
				break;
			}
			catch(bad_lexical_cast&)
			{
				std::cout << "Type mismatch. Please try again." << std::endl;
			}
			catch(std::exception& e)
			{
				std::cout << "Bad value (" << e.what() << "). Please try again." << std::endl;
			}
		}

	}
}

int Run()
{
	try
	{
		SettingsMgr mgr;
		mgr.Start();
		mgr.Save();

		std::cout << "Please set the following options. The value in square brackets is default." << std::endl; 
		std::cout << "Accept it by pressing enter or enter new value." << std::endl;
		std::cout << "Only essential options will be shown. To alter some specific options, edit sharelin.conf file manually." << std::endl;

		Options opts;
		LoadOptions(opts);

		Setup(opts, mgr);

		std::cout << "Saving settings" << std::endl;
		mgr.Save();
	}
	catch (std::exception& e)
	{
		std::cout << "Couldn't configure because of error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

} //namespace Configurer

