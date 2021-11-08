/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008  Andrey Stroganov <savthe@gmail.com>

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

#include <set>

#include <boost/foreach.hpp>

#include "localqht.hpp"
#include "util.hpp"
#include "../querylang.hpp"

namespace share_details {

void LocalQht::update(const FileInfo& f)
{
	timestamp_ = util::now();

	qht_.Add("urn:sha1:" + f.sha1.toString()); 
	qht_.Add("urn:ttr:" + f.ttr.toString()); 
	qht_.Add("urn:tree:tiger/:" + f.ttr.toString()); 
	qht_.Add("urn:ed2k:" + f.ed2k.toString()); 
	qht_.Add("urn:md5:" + f.md5.toString()); 

	std::set<std::string> kws;
	G2::ExtractKeywords(f.name, std::inserter(kws, kws.begin()));
	BOOST_FOREACH(const std::string& kw, kws) 
		qht_.Add(kw);
}

} //namespace share_details
