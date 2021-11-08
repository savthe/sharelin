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

#ifndef DL_TOOLS_HPP
#define DL_TOOLS_HPP

#include <set>

#include "../sharedfile.hpp"

namespace download_details {

void sync_incomplete_folder();	
void fix_missing_info(const std::string&);
void sync_missing_info(const std::set<std::string>&);
void fix_missing_file(SharedFile);
void sync_missing_files(const std::set<std::string>&);
void sync_incomplete_folder();

} // namespace share

#endif // DL_TOOLS_HPP

