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

#ifndef DIRECTORYWALK_HPP
#define DIRECTORYWALK_HPP

#include <vector>
#include <string>
#include <boost/function.hpp>

class DirectoryWalk
{
	typedef std::vector<std::string> Found;

public:
	typedef boost::function <bool (const std::string&)> Predicate;
	typedef Found::const_iterator iterator;

	iterator found_begin() const { return found_.begin(); }
	iterator found_end() const { return found_.end(); }
	size_t found_size() const { return found_.size(); }

	template <typename T> void assign(T begin, T end, Predicate pred)
	{
		folders_.assign(begin, end);
		found_.clear();
		predicate_ = pred;
	}

	void run();

private:
	Predicate predicate_;
	std::vector <std::string> folders_;
	Found found_;
};

#endif //DIRECTORYWALK_HPP

