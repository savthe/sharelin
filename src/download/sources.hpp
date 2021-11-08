/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009  Andrey Stroganov <savthe@gmail.com>

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

#ifndef DL_SOURCES_HPP
#define DL_SOURCES_HPP

#include <map>

#include "../sourcehost.hpp"

namespace download_details {

typedef std::map<ip::Endpoint, SourceHost> Sources;

/*
#include <vector>
#include <ctime>

#include <boost/iterator/transform_iterator.hpp>



class Sources
{
	typedef std::map<ip::Endpoint, SourceHost> Map;

	struct Adaptor: public std::unary_function<const Map::value_type&, const Map::mapped_type&>
	{
		const Map::mapped_type& operator() (const Map::value_type& v) const
		{
			return v.second;
		}
	};

public:
	typedef boost::transform_iterator<Adaptor, Map::const_iterator> iterator;

	void add(const SourceHost&);
	iterator find(const ip::Endpoint& e) const { return iterator(map_.find(e)); }
	iterator begin() const { return iterator(map_.begin()); }
	iterator end() const { return iterator(map_.end()); }
	std::size_t size() const { return map_.size(); }
	void connect_after(ip::Endpoint, std::time_t);

private:
	Map map_;
};
*/

} // namespace download_details

#endif // DL_SOURCES_HPP

