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

#include <boost/bind.hpp>

#include <boost/foreach.hpp>

#include "fetcher.hpp"
#include "../traf.hpp"
#include "io.hpp"
#include "../trafmanager.hpp"
#include "../verb.hpp"
#include "http_task.hpp"
#include "util.hpp"
#include "../share.hpp"

using util::since;
using util::now;
using boost::bind;

namespace download_details {

Fetcher::Fetcher(SharedFile& f, Validator& v, Sources& s):
	file_(f), 
	range_destribute_(file_->map(), downloaders_),
	traf_(app::traf_in()),
	active_(false),
	validator_(v),
	sources_(s),
	ticker_(app::io()),
	priority_(0)
	{
	}

void Fetcher::start()
{
	active_ = true;

	ticker_.tick(boost::posix_time::seconds(1), bind(&Fetcher::tick, this));
}

bool Fetcher::need_sources()
{
	return range_destribute_.need_sources();
}

void Fetcher::priority(int8 n)
{
	priority_ = n;

	BOOST_FOREACH(Downloader::Ptr p, downloaders_) 
		p->priority(n);
}

void Fetcher::tick()
{
	Downloaders ptrs(downloaders_);

	BOOST_FOREACH(Downloader::Ptr p, ptrs) 
	{
		const std::time_t timestamp = p->traf().last_activity();

		switch(p->state())
		{
			case dlr_state::QUEUED:
			{
				if(since(timestamp) > std::time_t(p->queue().pollMin) + 3)
					assign_task(p);
				break;
			}
			case dlr_state::TRANSFER:
			case dlr_state::CHAT:
			{
				if(since(timestamp) > 5)
				{
					verb2::dl << "Downloader " << p->endpoint() << " stalled " << std::endl;
					p->close();
					downloaders_.erase(p);
				}
				break;
			}
			case dlr_state::IDLE:
			case dlr_state::CLOSED:
			case dlr_state::CONNECT: assert(!"can't be here");
			{}
		}
	}

	ticker_.tick(boost::posix_time::seconds(1), bind(&Fetcher::tick, this));
}

void Fetcher::on_file(const bs::error_code& err, FileBufferPtr buf, Downloader::Ptr p)
{
	verb2::dl << "Downloader " << p->endpoint() << " has finished file fetching task: " << err.message() << std::endl;

	if(!err && active_)
	{
		file_->sync();
		assign_task(p);
	}
	else
		handle_error(err, p);
}

void Fetcher::handle_error(const bs::error_code& err, Downloader::Ptr p)
{
	if(err != ba::error::operation_aborted)
		p->close();

	downloaders_.erase(p);
}

void Fetcher::on_tiger(const bs::error_code& err, BytesBufferPtr buf, Downloader::Ptr p)
{
	verb2::dl << "Downloader " << p->endpoint() << " on_tiger: " << err.message() << std::endl;

	if(err || !active_)
	{
		handle_error(err, p);
		return;
	}

	try
	{
		TigerTree t;
		t.fromBytes(buf->bytes(), buf->size(), file_->size());
		file_->tiger_tree(t);

		verb2::dl << "Got valid tiger tree. Minimal range: " << t.block_size() << std::endl;
		//pFileMap_->SetVerificationSize(t.minimalRange());
	}
	catch (std::exception& err)
	{
		verb2::dl << "Failed to import tiger tree: " << err.what() << std::endl;
	}

	assign_task(p);
}

void Fetcher::stop()
{
	BOOST_FOREACH(Downloader::Ptr p, downloaders_) 
		p->close();

	ticker_.cancel();
	downloaders_.clear();
	active_ = false;
}

void Fetcher::insert(Downloader::Ptr p)
{
	assert(active_);
	downloaders_.insert(p);
	p->priority(priority_);
	assign_task(p);
}

void Fetcher::assign_task(Downloader::Ptr p)
{
	verb2::dl << "Assigning task to: " << p->endpoint() << std::endl;

	bool assigned = false;

	if(!p->tiger_uri().empty() && file_->tiger_tree().empty())
	{
		verb2::dl << "Assigning tiger fetch: " << p->endpoint() << std::endl;

		HttpTask t(HttpTask::FETCH_TIGER, p->tiger_uri());
		t.accept_type = "application/tigertree-breadthfirst";

		BytesBufferPtr buf(new BytesReceiveBuffer);
		p->fetch(t, *buf, bind(&Fetcher::on_tiger, shared_from_this(), _1, buf, p));

		assigned = true;
	}
	else
	{
		Sources::iterator i = sources_.find(p->endpoint());
		std::string uri;
		if(i != sources_.end()) uri = i->second.uri;
		if(uri.empty()) uri = "/uri-res/N2R?urn:sha1:" + file_->sha1().toString(); //FIXME what if we don't have sha1?

		HttpTask t(HttpTask::FETCH_FILE, uri);
		ByteRange r = range_destribute_.select_range(p);
		if(!r.empty()) 
		{
			verb2::dl << "Assigning fetching of file range " << r.first() << "-" << r.last() << " to " << p->endpoint() << std::endl;
			t.range = r;
			FileBufferPtr buf(new FileReceiveBuffer(file_));
			p->fetch(t, *buf, bind(&Fetcher::on_file, shared_from_this(), _1, buf, p));

			assigned = true;
		}
	}

	if(!assigned) 
	{
		verb2::dl << "No work for: " << p->endpoint() << std::endl;
		p->close();
		downloaders_.erase(p);
	}
}

} // namespace download_details
