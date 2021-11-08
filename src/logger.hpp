/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010 Andrey Stroganov <savthe@gmail.com>

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

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <string>
#include <boost/noncopyable.hpp>

class Logger: public boost::noncopyable
{
public:
	static Logger* Instance() 
	{
		if(!pSelf_) pSelf_ = new Logger;
		return pSelf_;
	}

	bool Init(const std::string&, unsigned int, unsigned int v = 0);
	void Verbose(unsigned int v) { verb_ = v; };
	void Shutdown();
	void Log(unsigned int, const std::string&);

private:
	Logger() {}

	bool InitFile();
	void UpdateFile();

	std::string name_;
	unsigned int maxSize_;
	unsigned int verb_;
	std::fstream file_;

	static Logger* pSelf_;
};


#endif //LOGGER_HPP

