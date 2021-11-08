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

#ifndef UIQUERY_HPP
#define UIQUERY_HPP

#include <string>
#include <stdexcept>
#include <sstream>
#include <map>

#include <boost/lexical_cast.hpp>

#include "tag.hpp"

namespace ui {

class Query
{

public:
	typedef std::map <std::string, std::string> Container;
	typedef Container::const_iterator Iterator;

	class NotFound: public std::runtime_error
	{
	public:
		NotFound(const std::string& s): std::runtime_error("Query error. Value not found: " + s) {}
	};

	void FromString(const std::string& s, char sep = ' ');	
	void FromString(const std::string& sCmd, const std::string& sParams, char sep = ' ');
	void Remove(const std::string& key);
	void Remove(const Tags::Type key);
	void Reset();
	bool Has(const std::string&) const;
	bool Has(Tags::Type) const;
	bool IsFlag(const std::string&) const;
	bool IsFlag(Tags::Type ) const;
	bool HasValues() const { return !container_.empty(); }
	Tags::Type GetType() const { return query_; } 
	Iterator Begin() const { return container_.begin(); }
	Iterator End() const { return container_.end(); }
	Iterator Find(const std::string& key) const { return container_.find(key); }
	Iterator Select(const std::string&) const;

	template <class T>
	T Get(const std::string& key) const
	{
		return boost::lexical_cast<T>(Select(key)->second);
	}

	template <class T>
	T Get(Tags::Type tag) const
	{
		return Get<T>(StaticName::toString(tag));
	}

private:
	Container container_;
	Tags::Type query_;
};

} //namespace ui

#endif //UIQUERY_HPP

