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

#ifndef STREAMLOG_HPP
#define STREAMLOG_HPP

#include <boost/noncopyable.hpp>

#include "logger.hpp"
#include <string>
#include <ostream>
#include <sstream>

class StreamLog: public boost::noncopyable
{
public:
	StreamLog(int level, std::string prefix = std::string()): level_(level), prefix_(prefix), buffer_(prefix) {}
	void reset() { buffer_ = prefix_; }
	void append(const std::string& s) { buffer_.append(s); }
	std::string message() const { return buffer_; }
	int level() const { return level_; }

private:
	int level_;
	std::string prefix_;
	std::string buffer_;

	template <typename T> friend StreamLog& operator<<(StreamLog&, const T&);
	friend StreamLog& operator<<(StreamLog& s, std::ostream& (*fn)(std::ostream&));
};

StreamLog& operator<<(StreamLog& s, std::ostream& (*f)(std::ostream&));

template <typename T>
StreamLog& operator<<(StreamLog& s, const T& data)
{
	std::stringstream ss;
	ss << data;
	s.append(ss.str());
	return s;
}

#endif //STREAMLOG_HPP

