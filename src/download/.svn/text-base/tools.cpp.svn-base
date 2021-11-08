/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012 Andrey Stroganov <savthe@gmail.com>

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

#include <fstream>

#include <boost/algorithm/string.hpp>

#include "tools.hpp"
#include "verb.hpp"
#include "filesystem.hpp"
#include "share.hpp"
#include "settings.hpp"

using FileSystem::FileName;
using FileSystem::FileSize;
using boost::starts_with;
using boost::istarts_with;

namespace download_details {

void fix_missing_info(const std::string& path)
{
	const std::string name = FileName(path);

	FileInfo i;
	i.path = path;

	if(istarts_with(name, "sha1_")) i.sha1.fromString(name.substr(5));
	else if(istarts_with(name, "ttr_")) i.ttr.fromString(name.substr(4));
	else if(istarts_with(name, "md5_")) i.md5.fromString(name.substr(4));
	else if(istarts_with(name, "ed2k_")) i.ed2k.fromString(name.substr(5));
	else throw std::runtime_error("couldn't resolve hash type");

	i.size = FileSize(path);
	i.partial = true;

	app::share().add(i);
}

void sync_missing_info(const std::set<std::string>& files)
{
	verb1::dl << "Looking for missing info" << std::endl;

	for(std::set<std::string>::iterator i = files.begin(); i != files.end(); ++i)
	{
		const std::string& path = *i;
		SharedFile f = app::share().findPath(path);
		if(!f)
		{
			verb1::dl << "Missing information about " << path << " . Fixing." << std::endl;
			try { fix_missing_info(path); }
			catch (std::exception& e)
			{
				verb1::dl << "Couldn't fix missing info for: " << path << " Problem: " << e.what() << std::endl; 
				verb1::dl << "You'll have to download this file manually" << std::endl; 
			}
		}
	}
}

void fix_missing_file(SharedFile f)
{
	verb1::dl << "Missing file " << f->path() << " . Creating." << std::endl;
	f->create();
}

void sync_missing_files(const std::set<std::string>& files)
{
	verb1::dl << "Looking for missing files" << std::endl;

	std::vector<SharedFile> v;
	app::share().dump(v);
	for(size_t i = 0; i < v.size(); ++i)
		if(starts_with(v[i]->path(), app::settings().general.incomplete) && files.find(v[i]->path()) == files.end())
			fix_missing_file(v[i]);
}


void sync_incomplete_folder()
{
	verb1::dl << "Syncronizing incomplete folder" << std::endl;

	typedef std::set<std::string> Files;
	Files files;
	ScanDirectory(app::settings().general.incomplete, std::inserter(files, files.begin()), FileSystem::REGULAR_FILE, FileSystem::NOT_FOLLOW);

	sync_missing_info(files);
	sync_missing_files(files);
}

} // namespace download_details

