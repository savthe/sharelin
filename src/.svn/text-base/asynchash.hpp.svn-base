/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2010  Andrey Stroganov <savthe@gmail.com>

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

#ifndef HASHER_HPP
#define HASHER_HPP

#include <string>

#include <boost/function.hpp>
#include <boost/system/error_code.hpp>

#include "hashes.hpp"
#include "types.hpp"
#include "byterange.hpp"
#include "async_work.hpp"
#include "tigertree.hpp"
#include "tiger_tree_composer.hpp"

class AsyncTaskServer;

class AsyncHash
{
public:
	typedef boost::function<void(const boost::system::error_code&)> Handler;

	AsyncHash(AsyncTaskServer&);

	void run(const std::string&, hash::Mask::Type, ByteRange, size_t rate, Handler);
	void cancel();
	bool active() const { return work_.has_task(); }

	std::string path() const { return path_; }
	hash::Mask::Type mask() const { return mask_; }
	ByteRange range() const { return range_; }
	size_t rate() const { return actual_rate_; }
	hash::SHA1 sha1() const { return sha1_; }
	hash::MD5 md5() const { return md5_; }
	hash::TTR ttr() const { return ttr_; }
	hash::ED2K ed2k() const { return ed2k_; }
	TigerTree tiger_tree() const { return tiger_tree_; }

private:
	void done();
	void async_run();
	void async_safe_run();

	AsyncWork work_;
	Handler handler_;
	bool cancel_;
	boost::system::error_code error_;

	std::string path_;
	hash::Mask::Type mask_;
	ByteRange range_;
	size_t rate_; //bps
	hash::SHA1 sha1_;
	hash::TTR ttr_;
	hash::MD5 md5_;
	hash::ED2K ed2k_;
	TigerTree tiger_tree_;
	size_t actual_rate_;

};

#endif //HASHER_HPP

