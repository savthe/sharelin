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

#ifndef DL_DOWNLOAD_HPP
#define DL_DOWNLOAD_HPP

#include <stdexcept>
#include <ctime>
#include <memory>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "types.hpp"
#include "../file.hpp"
#include "asynchash.hpp"
#include "asynccopy.hpp"
#include "ip.hpp"
#include "guid.hpp"
#include "sourcehost.hpp"
#include "inf/download.hpp"
#include "states.hpp"
#include "io.hpp"
#include "../sharedfile.hpp"

namespace download_details {

class Manager;
class Transfer;

class Download: public boost::enable_shared_from_this<Download>
{
public:
	struct BadStatus: public std::runtime_error
	{
		BadStatus(const char* msg): std::runtime_error(msg) {}
	};

	//enum PriorityType {LOW = 0, BELOW, NORMAL, ABOVE, HIGH};

	Download(Manager&, SharedFile);
	~Download();
	dl_state::state_t state() const { return state_; }
	int8 priority() const;
	void priority(int8); 
	Id id() const { return file_->partial(); }
	const FileInfo& GetFileInfo() const { return file_->info(); }
	std::string DefaultURN() const;

	bool has_source(Guid guid) const;
	void onGiv(TcpSocketPtr pSock, ip::Endpoint endpoint);
	void Open(const std::string& path);
	void resume();
	void save();
	void stop();
	void pause();
	void cancel();
	void add_source(const SourceHost& host);
	inf::Download info() const;
	void load();
	void shutdown(); 

private:
	void finalize();
	void on_hashed(const boost::system::error_code&);
	void on_copied(const boost::system::error_code&);
	void DetachMe();
	void on_transferred();
	void init_file(file_offset_t size);

	std::string InfPath();

	Manager& manager_;
	SharedFile file_;
	std::auto_ptr<Transfer> transfer_;
	dl_state::state_t state_;
	AsyncHash async_hash_;
	AsyncCopy async_copy_;
};

} // namespace download_details

#endif //DL_DOWNLOAD_HPP

