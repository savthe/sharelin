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

#include <ctime>

#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "asynchash.hpp"
#include "hash/sha1.h"
#include "hash/ttr.h"
#include "hash/md5.h"
#include "hash/ed2khash.h"
#include "asyncfile.hpp"
#include "async_task_server.hpp"

AsyncHash::AsyncHash(AsyncTaskServer& s):
	work_(s)
{
}

void AsyncHash::run(const std::string& path, hash::Mask::Type mask, ByteRange range, size_t rate, Handler handler)
{
	path_ = path;
	mask_ = mask;
	range_ = range;
	rate_ = rate;
	cancel_ = false;
	error_.clear();
	handler_ = handler;

	work_.run(boost::bind(&AsyncHash::async_run, this), boost::bind(&AsyncHash::done, this), "Hash " + path); 
}

void AsyncHash::done()
{
	Handler h;
	h.swap(handler_);
	h(error_);
}

void AsyncHash::async_run()
{
	try
	{ 
		async_safe_run();
	}
	catch( boost::system::system_error& e )
	{
		error_ = e.code();
	}
}

void AsyncHash::async_safe_run()
{
	const int max_depth = 9;
	AsyncFile f;
	f.open(path_, fio::READ); 
			
 	SHA1Context 	ctx_sha1;
 	md5_state_t 	ctx_md5;
 	TT_CONTEXT 	ctx_ttr;
 	ed2k_ctx_t	ctx_ed2k;
	TigerTreeComposer tt;
 
 	SHA1Reset	( &ctx_sha1 );
 	md5_init	( &ctx_md5 );
 	tt_init		( &ctx_ttr );
 	ed2k_init 	( &ctx_ed2k );
	tt.init(f.size(), max_depth);

	const bool doSHA1 = mask_ & hash::Mask::SHA1;
	const bool doTTR = mask_ & hash::Mask::TTR;
	const bool doMD5 = mask_ & hash::Mask::MD5;
	const bool doED2K = mask_ & hash::Mask::ED2K;
	const bool doTigerTree = mask_ & hash::Mask::TT;

	const file_offset_t BUF_SIZE = 1024;
    char buf[BUF_SIZE];

	uint amount = 0;
	std::time_t now = std::time(0);
	std::time_t startTime = std::time(0);
 
	for(file_offset_t i = 0; i < range_.length();)
	{
		if(cancel_) return;

		const file_offset_t n = std::min(BUF_SIZE, range_.length() - i);

		f.read(buf, n, range_.offset() + i); 

 		if(doSHA1) SHA1Input(&ctx_sha1, (uint8_t*)buf, n);
 		if(doMD5) md5_append(&ctx_md5, (md5_byte_t*)buf, n);
 		if(doTTR) tt_update(&ctx_ttr, (byte*)buf, n);
 		if(doED2K) ed2k_update(&ctx_ed2k, (unsigned char*)buf, n);
		if(doTigerTree) tt.update(buf, n);

		i += n;
		amount += n;

		if(rate_ && amount >= rate_)
		{
			while(now == std::time(0))
				boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			now = std::time(0);
			amount = 0;
		}
	}

	const std::time_t duration = std::time(0) - startTime;
	actual_rate_ = range_.length() / (duration + 1);

	f.close();

	assert(hash::SHA1::size() <= BUF_SIZE);
	assert(hash::TTR::size() <= BUF_SIZE);
	assert(hash::MD5::size() <= BUF_SIZE);
	assert(hash::ED2K::size() <= BUF_SIZE);

	if(doSHA1)
	{
		SHA1Result(&ctx_sha1, (uint8_t*)buf);
		sha1_.assign(buf);
	}

	if(doMD5)
	{
		md5_finish(&ctx_md5, (md5_byte_t*)buf);
 		md5_.assign(buf);
	}
 
	if(doTTR)
	{
		tt_digest(&ctx_ttr, (byte*)buf);
		ttr_.assign(buf);
	}
 
 	if(doED2K)
	{
		ed2k_finish(&ctx_ed2k, (byte*)buf);
		ed2k_.assign(buf);
	}

	if(doTigerTree)
	{
		tt.digest(tiger_tree_);
	}
}

void AsyncHash::cancel()
{
	cancel_ = true;
	work_.cancel();
	Handler h;
	h.swap(handler_);
}

