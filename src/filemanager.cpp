/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2010  Andrey Stroganov <savthe@gmail.com>

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

#include "filemanager.hpp"
#include "debug.hpp"

void FileManager::Transfer(const std::string& from, const std::string& to)
{
	Pointers::iterator i = pointers_.find(from);	
	if(i == pointers_.end()) return;
	AsyncFilePtr p = i->second;
	fio::mode_t mode = p->mode();
	p->close();
	try { p->open(to, mode); }
	catch( std::exception& e )
	{
		p->open(from, mode);
		throw;
	}
}

File FileManager::Open(const std::string& path, fio::mode_t mode)
{
	AsyncFilePtr p;

	Pointers::iterator i = pointers_.find(path);
	if(i != pointers_.end())
	{
		p = i->second;
		if((mode & i->second->mode()) != mode)
		{
			fio::mode_t oldMode = p->mode();
			p->close();
			try {
				p->open(path, mode);
			}
			catch (fio::error::SystemError&)
			{
				p->open(path, oldMode);
				throw;
			}
		}
	}
	else
	{
		p = AsyncFilePtr(new fio::AsyncFile);
		p->open(path, mode);
		pointers_[path] = p;
	}

	return File(this, p);
}

void FileManager::Close(AsyncFilePtr& pFile)
{
	//PRINT2("Closing shared file with refs:", pFile.use_count());
	pFile.reset();
	for(Pointers::iterator i = pointers_.begin(); i != pointers_.end(); ++i)
	{
		if(i->second.unique())
		{
			i->second->close();
			pointers_.erase(i);
			break;
		}
	}

	//PRINT("Close finished");
}

