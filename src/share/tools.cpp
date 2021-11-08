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

#include <boost/algorithm/string.hpp>

#include "tools.hpp"
#include "../verb.hpp"

using boost::iequals;
using boost::trim;

namespace share_details {

std::ostream& operator<<(std::ostream& os, const FileMap& m)
{
	os << m.file_size() << ' ';
	for(FileMap::iterator i = m.begin(); i != m.end(); ++i)
	{
		os << i->first() << "-" << i->last() << ":";
		switch (i->color().value())
		{
			case Integrity::VALID: os << 'V'; break;
			case Integrity::INVALID: os << 'I'; break;
			case Integrity::EMPTY: os << 'E'; break;
			case Integrity::FULL: os << 'F'; break;
		}
		os << ';';
	}

	return os;
}

std::ostream& operator<<(std::ostream& os, const TigerTree& t)
{
	std::vector <char> v(t.size_in_bytes());
	t.to_bytes(v.begin());

	std::string s;
	conv::Base32::encode(v.begin(), v.end(), std::back_inserter(s));

	os << s;

	return os;
}

std::ostream& operator<<(std::ostream& os, const FileInfo& inf)
{
	os << "[file]" << "\n";
	os << "path: " << inf.path << "\n";
	os << "name: " << inf.name << "\n";
	if(!inf.sha1.empty()) os << "sha1: " << inf.sha1.toString() << "\n";
	if(!inf.ttr.empty()) os << "ttr: " << inf.ttr.toString() << "\n";
	if(!inf.md5.empty()) os << "md5: " << inf.md5.toString() << "\n";
	if(!inf.ed2k.empty()) os << "ed2k: " << inf.ed2k.toString() << "\n";
	os << "size: " << inf.size << "\n";
	os << "hashed: " << inf.hashed << "\n";
	os << "hits: " << inf.hits << "\n";
	os << "requests: " << inf.requests << "\n";
	if(!inf.tiger_tree.empty()) os << "tigertree: " << inf.size << " " << inf.tiger_tree << "\n"; //small workaround. FIXME
	if(!inf.map.empty()) os << "map: " << inf.map << "\n";
	os << "[/file]" << std::endl;
	
	return os;
}

std::istream& operator>>(std::istream& is, FileMap& m)
{
	m.clear();

	file_offset_t size;
	if(!(is >> size)) throw std::runtime_error ("map format error");
	m.init(size);

	file_offset_t offset = 0;
	file_offset_t first, last;
	char color;

	char sep1, sep2, sep3;
	while(is >> first >> sep1 >> last >> sep2 >> color >> sep3)
	{
		//std::cout << "L: " << first << " " << last << " " << offset << std::endl;
		if(sep1 != '-' || sep2 != ':' || sep3 != ';')
			throw std::runtime_error("file map format error");

		if(first != offset || last < first) 
			throw std::runtime_error("Bytes range hole");

		const ByteRange r(first, last - first + 1);

		switch(color)
		{
			case 'v': case 'V': m.set_valid(r); break;
			case 'e': case 'E': m.set_empty(r); break;
			case 'i': case 'I': m.set_invalid(r); break;
			case 'f': case 'F': m.set_full(r); break;
			default: throw std::runtime_error("bad fragment color");
		}
		offset = last + 1;
	}

	return is;
}

std::istream& operator>>(std::istream& is, TigerTree& t)
{
	std::string s;
	uint64 size;
	is >> size >> s;

	if(!s.empty())
	{
		std::vector<char> v;
		conv::Base32::decode(s.begin(), s.end(), std::back_inserter(v));
		t.fromBytes(v.begin(), v.size(), size);
	}

	return is;
}

void read_line(std::istream& is, std::string& s)
{
	getline(is, s);
	trim(s);
	for(size_t i = 0; i < s.size(); ++i)
		if(s[i] == ':')
		{
			s[i] = ' ';
			break;
		}
}

std::istream& operator>>(std::istream& istr, FileInfo& f)
{
	f = FileInfo();
	std::string s; 
	while((istr >> s) && !iequals(s, "[file]"));

	while(istr)
	{
		std::string line;
		read_line(istr, line);
		std::istringstream is(line);
		is >> s;
		if(s == "[/file]") break;

		if(iequals(s, "path"))
		{
			getline(is, f.path); 
			trim(f.path);
		}
		else if(iequals(s, "name"))
		{
			getline(is, f.name); 
			trim(f.name);
		}
		else if(iequals(s, "sha1")) is >> f.sha1;	
		else if(iequals(s, "ttr")) is >> f.ttr;	
		else if(iequals(s, "md5")) is >> f.md5;	
		else if(iequals(s, "ed2k")) is >> f.ed2k;	
		else if(iequals(s, "hits")) is >> f.hits;
		else if(iequals(s, "size") && !(is >> f.size)) f.size = 0;
		else if(iequals(s, "requests") && !(is >> f.requests)) f.requests = 0;
		else if(iequals(s, "hashed") && !(is >> f.hashed)) f.hashed = 0;
		else if(iequals(s, "tigertree")) is >> f.tiger_tree;
		else if(iequals(s, "map")) is >> f.map;

	}

	return istr;
}

} // namespace share_details

