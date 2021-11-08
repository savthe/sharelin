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

#ifndef SETTINGSSTORAGE_HPP
#define SETTINGSSTORAGE_HPP

#include <vector>

#include "ipsegment.hpp"
#include "types.hpp"
#include "settings_types.hpp"

struct SettingsStorage
{
	struct General
	{
		std::string userName;
		std::string incomplete;
		std::string complete;
		std::vector<IpSegment> uisegments;
	} general;
	
	struct Net 
	{
		uint maxHubs;
		uint maxBwIn;
		uint maxBwOut;
		uint16 listenPort;
		ip::Address listenIp;
		uint maxUploads;
		uint maxDownloads;
		uint maxFiles;
		uint queueSize;
		uint pollMin;
		uint pollMax;
		uint uploadsPerHost;
		settings_types::firewalled::type firewalled; //0-auto, 1-yes, 2-no
		uint qkeyPeriod;
	} net;
	
	struct Share
	{
		std::vector<std::string> folders;
		uint hashRate;
	} share;
	
	struct Telnet
	{
		std::string style;
		uint16 port;
		uint pageSize;
	} telnet;

	struct Webui
	{
		std::string style;
		uint16 port;
		uint pageSize;
	} webui;
};

#endif //SETTINGSSTORAGE_HPP

