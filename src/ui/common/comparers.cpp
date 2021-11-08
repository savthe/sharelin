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

#include <stdexcept>
#include <algorithm>

#include "comparers.hpp"
#include "tools.hpp"

namespace ui {
namespace Comparers {

bool Download::operator()(const inf::Download& p1, const inf::Download& p2) const
{
	bool r = false;
	switch(opt_.SortBy())
	{
		case Tags::name: r = p1.f.title() < p2.f.title(); break;
		case Tags::size: r = p1.f.size < p2.f.size; break;
		case Tags::done: r = DownloadProgress(p1.f.map) < DownloadProgress(p2.f.map); break;
		case Tags::rate: r = p1.rate < p2.rate; break;
		case Tags::sources: r = p1.downloaders.size() < p2.downloaders.size(); break;
		case Tags::priority: r = p1.priority < p2.priority; break;
		default: r = p1.f.title() < p2.f.title();
	}

	return opt_.Forward() ? r : !r;
}

bool SharedFiles::operator()(const FileInfo* e1, const FileInfo* e2) const
{
	bool less;

	switch(opt_.SortBy())
	{
		case Tags::name: less = e1->title() < e2->title(); break;
		case Tags::size: less = e1->size < e2->size; break;
		case Tags::requests: less = e1->requests < e2->requests; break;
		case Tags::hits: less = e1->hits < e2->hits; break;
		default: less = e1->title() < e2->title();
	}

	less = opt_.Forward() ? less : !less;
	return less;
//	if(!opt_.Forward()) less = !less;
}

} //namespace Comparers
} //namespace ui
