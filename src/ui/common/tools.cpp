/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov

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

#include <cctype>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include "tools.hpp"
#include "hashes.hpp"
#include "app.hpp"
#include "filesystem.hpp"
#include "appdefs.hpp"
#include "verb.hpp"

using FileSystem::Exists;
using boost::lexical_cast;

namespace ui {

//typedef std::vector<SearchResultList::iterator> ResultIterators;

std::string FindStyle(const std::string& style, const std::string& parent)
{
	const std::string path = parent + "/" + style;
	std::set<std::string> paths;
	paths.insert(app::home());
	paths.insert("/usr/local/share/sharelin");
	paths.insert("/usr/share/sharelin");
#ifdef DEFAULT_ROOT
	paths.insert(std::string(DEFAULT_ROOT) + "/share/sharelin");
#endif

	std::string root;

	for(std::set<std::string>::iterator i = paths.begin(); i != paths.end(); ++i)
	{
		const std::string s = *i + "/" + path;
		verb1::ui << "Searching style '" << style << "' in " << s << " => ";
		if(Exists(s))
		{
			verb1::ui << "Found" << std::endl;
			root = s;
			break;
		}
		else verb1::ui << "Not found" << std::endl;
	}

	return root;
}

std::string TimeRange(std::time_t r)
{
	const int s = r % 60;
	const int m = (r / 60) % 60;
	const int h = (r / (60 * 60)) % 24;
	const int d = (r / (24 * 60 * 60)) % 365;

	std::stringstream out;
	if(d) out << d << "d ";
	if(h || d) out << h << "h ";
	if(m || h || d) out << m << "m ";
	out << s << "s";

	return out.str();
}

std::string HumanReadable(uint64 x)
{
	const uint64 kb = 1024;
	const uint64 mb = 1024 * 1024;
	const uint64 gb = 1024 * mb;
	uint t = 1;
	std::string prefix;

	if(x >= gb)
	{
		prefix = "GB";
		t = gb;
	}
	else if(x >= mb)
	{
		prefix = "MB";
		t = mb;
	}
	else if(x >= kb)
	{
		prefix = "KB";
		t = kb;
	}
	else
		prefix = "B";

	assert(t == 1 || t / 10);
	uint f = t == 1 ? 0 : ( x / (t / 10) ) % 10;

	/*
	uint64 a = x;
	uint64 t = 0;

	std::stringstream s;
	if (x < 1000) s << x << " B";
	else if (x < 1024*1024) s << x / 1024 << " KB";
	else if (x < 1024*1024*1024) s << x / (1024*1024) << " MB";
	else s << x / (1024*1024*1024) << " GB";
	*/
	std::stringstream s;
	s << x / t;
	if(f) s << "." << f;
	s << " " << prefix;
	return s.str();
}


uint64 GetSizeValue(const std::string& s) 
{
	std::size_t i = 0;
	while(i < s.size() && std::isdigit(s[i])) ++i;

	uint64 val = boost::lexical_cast<uint64>(s.substr(0, i));

	while(i < s.size() && s[i] == ' ') ++i;
	if(i == s.size()) return val;
	assert(i < s.size());

	if(s[i] == 'k' || s[i] == 'K') return val * 1024;
	if(s[i] == 'm' || s[i] == 'M') return val * 1024 * 1024;
	if(s[i] == 'g' || s[i] == 'G') return val * 1024 * 1024 * 1024;

	throw std::runtime_error("Bad size suffix");

	return val;
}

std::string RankedNumber(uint64 x, const std::string& separator)
{
	std::string s = lexical_cast<std::string>(x);
	std::string ss;
	for(int i = s.size() - 1, j = 1; i >= 0; --i, ++j) 
	{
		ss.insert(0, 1, s[i]);
		if(j % 3 == 0 && i > 0)
			ss = separator + ss;
	}
	return ss;	
}

std::string AdaptString(std::string s)
{
 	const uint max = 80;
 	//we want to ensure that there's a possibility (a space) in the line in every max sequence

 	uint count = 0;
 	uint not_alnum = 0;
 	for(uint i = 0; i < s.size(); i++) 
	{
 		if(s[i] == ' ') count = 0;
 		else count++;

 		if(!isalnum(s[i])) not_alnum = i;

 		if(count == max) 
		{
 			//let's add a space
			if(i - not_alnum >= max - 1 || not_alnum == 0) //not alnum is too far
 				not_alnum = i;
 			count = i - not_alnum;

 			if(not_alnum + 1 == s.size()) break;

 			s.insert(not_alnum + 1, " ");
 			not_alnum = 0;
 		}
 	}
 	return s;
}

file_offset_t count_bytes(const FileMap& m, Integrity::Color color)
{
	file_offset_t s = 0;

	for(FileMap::iterator i = m.begin(); i != m.end(); ++i)
		if(i->color() == color)
			s += i->length();

	return s;
}

uint DownloadProgress(const FileMap& m)
{
	file_offset_t s = 0;

	s += count_bytes(m, Integrity::FULL);
	s += count_bytes(m, Integrity::VALID);

	file_offset_t size = 0;
	for(FileMap::iterator i = m.begin(); i != m.end(); ++i)
		size += i->length();

	return Progress(s, size);
//	return Progress(p->GetFileMap()->CountBytes(integrity::FULL | integrity::VALID), p->GetFileMap()->CountBytes());
}

uint Progress(uint64 n, uint64 total)
{
	return total == 0 ? 0 : 100 * n / total;
}

std::string FileType(const std::string& name)
{
 	//std::string::size_type pos = name.find_last_of(".");
 	//if(pos >= name.size() - 1) return "unknown";

 	//std::string ext = boost::to_lower_copy(name.substr(pos + 1));
	return StaticName::toString(media::resolveByName(name));
}

uint CountPages(uint elements, uint perPage)
{
	return elements % perPage ? elements / perPage + 1 : elements / perPage;
}

std::string NameFromCriteria(const SearchCriteria& criteria)
{
	std::string dn = criteria.dn;
	if(!criteria.sha1.empty())
		dn = "SHA1 hash: " + criteria.sha1.toString();
	else if(!criteria.ttr.empty())
		dn = "TTR hash: " + criteria.ttr.toString();
	else if(!criteria.ed2k.empty())
		dn = "ED2K hash: " + criteria.ed2k.toString();
	else if(!criteria.md5.empty())
		dn = "MD5 hash: " + criteria.md5.toString();
	return dn;
}
} //namespace ui
