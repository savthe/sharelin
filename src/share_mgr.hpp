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

#ifndef SHARE_MGR_HPP
#define SHARE_MGR_HPP

#include <vector>

#include <boost/noncopyable.hpp>

#include "queryhashtable.hpp"
#include "sharedfile.hpp"

namespace share_details { class Manager; }

class ShareMgr: private boost::noncopyable
{
public:
	typedef std::vector<SharedFile> Files;

	ShareMgr();
	~ShareMgr();

	void shutdown();
	void start();
	void tick();
	void updateSettings();
	SharedFile add(const FileInfo&);
	//void update(const share::FileInfo&);
	void dump(Files&);
	void remove(const std::string&);
	void remove(Id);
	void sync();
	//void finalize(share::WeakInfo, const std::string&);
	//void requested(share::WeakInfo);
	//void hit(share::WeakInfo);

	SharedFile get(Id);
	SharedFile findPath(const std::string& path);
	SharedFile find_urn(const std::string&);
	SharedFile findHash(const hash::SHA1&);
	SharedFile findHash(const hash::TTR&);
	SharedFile findHash(const hash::MD5&);
	SharedFile findHash(const hash::ED2K&);
	SharedFile findHashes(const hash::SHA1&, const hash::TTR&, const hash::ED2K&, hash::MD5&);
	void findDn(const std::string&, Files&, std::size_t maxResults = 0);

	const QueryHashTable& qht();
	std::time_t qhtTime();

private:
	share_details::Manager* impl_;
};

#endif //SHARE_MGR_HPP

