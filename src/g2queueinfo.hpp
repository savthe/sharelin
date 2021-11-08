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

#ifndef G2QUEUEINFO_HPP
#define G2QUEUEINFO_HPP

#include <ctime>
#include <string>

namespace g2 {

struct QueueInfo
{
	QueueInfo(): position(0), length(0), limit(0), pollMin(0), pollMax(0), timestamp(0) {}
	std::string Header() const;
	void FromString( const std::string& );
	bool Enqueued() const { return position; }
	void Reset();

	unsigned int position;
	unsigned int length;
	unsigned int limit;
	unsigned int pollMin;
	unsigned int pollMax;
	std::string id;
	std::time_t timestamp;
};

} //namespace G2

#endif //G2QUEUEINFO_HPP

