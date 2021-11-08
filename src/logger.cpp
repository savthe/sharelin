/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011 Andrey Stroganov <savthe@gmail.com>

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
#include <ctime>

#include "filesystem.hpp"
#include "logger.hpp"

using FileSystem::CopyFile;
using FileSystem::Exists;
using FileSystem::Unlink;

Logger* Logger::pSelf_ = 0;

namespace {

const char* Timestamp()
{
	std::time_t rawtime;
  	struct tm * timeinfo;
  	static char buffer[80];

  	time ( &rawtime );
  	timeinfo = localtime ( &rawtime );

  	strftime (buffer, 80, "[%H:%M:%S]", timeinfo);
	return buffer;
}

} //namespace 

/*
Logger& operator<<(Logger& log, std::ostream& (*f)(std::ostream&))
{
	if(log.level_ <= log.verb_) f(std::cout);
	if(log.file_.is_open()) f(log.file_);
	log.Update();

	return log;
}
*/

void Logger::UpdateFile()
{
	if(!file_) return;
	if(file_.tellg() < maxSize_) return;

	file_.close();
	const std::string bak = name_ + ".bak";
	if(Exists(bak)) Unlink(bak); 
	CopyFile(name_, name_ + ".bak");

	InitFile();
}

bool Logger::Init(const std::string& fn, unsigned int sz, unsigned int v)
{
	name_ = fn;
	maxSize_ = sz;
	verb_ = v;
	return InitFile();
}

bool Logger::InitFile()
{
	file_.open(name_.c_str(), std::fstream::out);
	return file_.good();
}

void Logger::Log(unsigned int level, const std::string& msg)
{
	file_ << level << " " << Timestamp() << " " << msg << std::endl;
	if(!file_) std::cout << "Logging error. File is broken." << std::endl;
	if(level <= verb_) std::cout << Timestamp() << " " << msg << std::endl;
}

void Logger::Shutdown()
{
	if(file_.is_open()) file_.close();
}
