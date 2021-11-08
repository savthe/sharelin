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

#ifndef UICONTROLTOOLS_HPP
#define UICONTROLTOOLS_HPP

#include <ctime>

#include <boost/bind.hpp>

#include <search/search_criteria.hpp>

#include "ui/common/template.hpp"
#include "ui/common/sortoption.hpp"
#include "filemap.hpp"
#include "fileinfo.hpp"
#include "sharedfile.hpp"
#include "search/defs.hpp"

namespace ui {

std::string HumanReadable(uint64 x);
std::string TimeRange(std::time_t);
std::string FileType(const std::string& name);
std::string AdaptString(std::string s);
uint Progress(uint64 n, uint64 total);
uint DownloadProgress(const FileMap&);
uint64 GetSizeValue(const std::string& s);
std::string RankedNumber(uint64 x, const std::string& separator);
uint CountPages(uint elements, uint perPage);
std::string NameFromCriteria(const SearchCriteria& criteria);
std::string FindStyle(const std::string& style, const std::string& parent);
file_offset_t count_bytes(const FileMap&, Integrity::Color);

template <typename T>
std::string FormatExtensions(T begin, T end)
{
	std::stringstream s;
	for(T i = begin; i != end; ++i)
	{
		if(i != begin) s << ", ";

		s << *i;
	}

	return s.str();
}

struct Writer
{
	Writer(Template* p): p_(p) {}
	template <typename T1, typename T2>
	void Put(const T1& key, const T2& value)
	{
		p_->Put(key, value);
	}
	Template* p_;
};

	
struct ShareWriter: public Writer
{
	ShareWriter(Template* p): Writer(p) {}

	void operator()(const FileInfo* i)
	{
		Put("name", i->title());
		Put("path", i->path);
		Put("size", HumanReadable(i->size));
		Put("actual_size", i->size);
		Put("type", FileType(i->title()));
		Put("hits", i->hits);
		Put("requests", i->requests);
		Put("sha1", i->sha1.toString());
		Put("ttr", i->ttr.toString());
		Put("md5", i->md5.toString());
		Put("ed2k", i->ed2k.toString());
	}
};

template <typename Iter, typename Output> void PagedOutput(uint page, uint perPage, Iter begin, Iter end, Output out)
{
	const uint lower = page * perPage;
	const uint upper = (page + 1) * perPage - 1;
	uint count = 0;
	for(Iter i = begin; i != end; ++i, ++count)
		if(count >= lower && count < upper) out(*i);
}

} //namespace ui

#endif //UICONTROLTOOLS_HPP

