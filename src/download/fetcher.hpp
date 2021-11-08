/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012  Andrey Stroganov <savthe@gmail.com>

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

#ifndef DL_FETCHER_HPP
#define DL_FETCHER_HPP

#include <memory>

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#include "downloaders.hpp"
#include "range_destribute.hpp"
#include "bytes_receive_buffer.hpp"
#include "file_receive_buffer.hpp"
#include "../traf_client.hpp"
#include "downloader.hpp"
#include "../sharedfile.hpp"
#include "io.hpp"
#include "validator.hpp"
#include "sources.hpp"
#include "ticker.hpp"

namespace download_details {

class Fetcher: public boost::enable_shared_from_this<Fetcher>, public boost::noncopyable
{
public:
	typedef boost::shared_ptr<Fetcher> Ptr;
	Fetcher(SharedFile&, Validator&, Sources&);

	void start();
	void stop();
	void insert(Downloader::Ptr);
	const Downloaders& downloaders() const { return downloaders_; }
	bool active() const { return !downloaders_.empty(); }
	bool need_sources(); 
	int8 priority() const { return priority_; }
	void priority(int8);
	
private:
	typedef boost::shared_ptr<BytesReceiveBuffer> BytesBufferPtr;
	typedef boost::shared_ptr<FileReceiveBuffer> FileBufferPtr;

	void on_file(const bs::error_code&, FileBufferPtr, Downloader::Ptr);
	void handle_error(const bs::error_code&, Downloader::Ptr);
	void on_tiger(const bs::error_code&, BytesBufferPtr, Downloader::Ptr);
	void assign_task(Downloader::Ptr);
	void tick();

	SharedFile& file_;
	Downloaders downloaders_;
	RangeDestribute range_destribute_;
	TrafClient traf_;
	bool active_;
	Validator& validator_;
	Sources& sources_;
	Ticker ticker_;
	int8 priority_;
};

} // namespace download_details

#endif // DL_FETCHER_HPP


