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

#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP

#include <string>

#include <boost/noncopyable.hpp>

#include "settings_storage.hpp"

class SettingsMgr: private boost::noncopyable
{
public:
	void Save();
	void Load();
	void Start();
	void SetOption(const std::string&, const std::string&);
	const SettingsStorage& storage() const { return storage_; }
	void SetStorage(const SettingsStorage&);

private:
	void SetDefaultValues();
	void SetOptionFirewalled(const std::string&);
	void SetOptionComplete( const std::string&);
	void SetOptionIncomplete( const std::string&);
	void SetOptionShare(const std::string&);
	void SetOptionRemoteRanges(const std::string&);

	SettingsStorage storage_;
};

#endif //SETTINGSMANAGER_HPP

