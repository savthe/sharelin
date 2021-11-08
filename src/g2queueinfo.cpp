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

#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "g2queueinfo.hpp"
#include "util.hpp"

using util::split2;
using boost::iequals;
using boost::lexical_cast;

namespace g2 {

std::string QueueInfo::Header() const
{
	std::stringstream s;
	s << "X-Queue: position=" << position
		<< ",length=" << length
		<< ",limit=" << limit
		<< ",pollMin=" << pollMin
		<< ",pollMax=" << pollMax
		<< ",id=" << id;
	return s.str();
}

void QueueInfo::FromString( const std::string& str )
{
	Reset();

	std::stringstream s(str);
	std::string part;
	while(std::getline(s, part, ','))
	{
		std::string left, right;
		split2(part, "=", left, right);
		if(iequals(left, "position")) position = lexical_cast<unsigned int>(right);
		else if(iequals(left, "length")) length = lexical_cast<unsigned int>(right);
		if(iequals(left, "limit")) limit = lexical_cast<unsigned int>(right);
		if(iequals(left, "pollmin")) pollMin = lexical_cast<unsigned int>(right);
		if(iequals(left, "pollmax")) pollMax = lexical_cast<unsigned int>(right);
		if(iequals(left, "id")) id = right;
	}
}

void QueueInfo::Reset()
{
	position = 0;
	length = 0;
	limit = 0;
	pollMin = 0;
	pollMax = 0;
	id.clear();
	timestamp = 0;
}

} //namespace G2
