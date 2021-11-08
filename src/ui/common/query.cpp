/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov <savthe@gmail.com>

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

#include "query.hpp" 
#include <boost/algorithm/string.hpp>
#include "url.hpp"

namespace ui {

inline std::string ReadStringItem(const char*& p, char sep) 
{
	while(*p && *p == sep) ++p; //Skip spaces
	std::string s;
	if(*p)
	{
		bool subtext = *p == '"';
	
		if(subtext) ++p;

		//while not end of string and *p != separator 
		while(*p) 
		{
			if(*p == '\\' && *(p + 1)) //if backslash, use the symbol after it
				s += *(++p);
			else s += *p;
		
			++p;
		
			if(*p == 0) break;
		
			if(subtext && *p == '"') 
			{
				++p;
				break;
			}
		
			if(!subtext && (*p == sep || *p == '='))
				break;
		}
	
		while(*p && *p == sep) ++p; //Skip spaces
	}
	
	return s;
}

void Query::FromString(const std::string& sq, const std::string& sParams, char sep)
{
	if(sq.empty()) throw std::runtime_error("Query is empty");
	if(sq.size() > 8) throw std::runtime_error("Query name is too large: " + sq);

	query_ = StaticName::fromString(boost::to_lower_copy(sq));

	container_.clear();

	const char* p = sParams.c_str();

	while(*p)
	{
		std::string strParam = ReadStringItem(p, sep);
		if(strParam.empty()) throw std::runtime_error("Parameter name is empty");

		std::string strVal;
		if(*p && *p == '=')
		{
			++p;
//			strVal = ReadStringItem(p, sep);
			if(*p)
			{
				if(*p != sep) strVal = ReadStringItem(p, sep);
				else ++p;
			}
		}

		container_[boost::to_lower_copy(strParam)] = Url::decode(strVal);
	}
}

Query::Iterator Query::Select(const std::string& key) const
{
	Iterator i = Find(key);
	if(i == End()) throw NotFound(key);
	return i;
}

void Query::Remove(const std::string& key)
{
	container_.erase(key);
}

void Query::Remove(const Tags::Type key)
{
	Remove(StaticName::toString(key));
}

void Query::Reset()
{
	container_.clear();
	query_ = 0;
}

void Query::FromString(const std::string& s, char sep)
{
	if(s.empty()) throw std::runtime_error("Empty string supplied");

	std::string::size_type pos = s.find(sep);

	if(pos < s.size())
		FromString(s.substr(0, pos), s.substr(pos + 1), sep);
	else 
	{
		if(s.size() > 8) throw std::runtime_error("Query type is too large");
		query_ = StaticName::fromString(boost::to_lower_copy(s));
	}
}

bool Query::Has(const std::string& key) const
{
	return container_.find(key) != container_.end();
}

bool Query::Has(Tags::Type tag) const
{
	return Has(StaticName::toString(tag));
}

bool Query::IsFlag(const std::string& key) const
{
	return Select(key)->second.empty();
}

bool Query::IsFlag(Tags::Type tag) const
{
	return IsFlag(StaticName::toString(tag));
}

} //namespace ui
