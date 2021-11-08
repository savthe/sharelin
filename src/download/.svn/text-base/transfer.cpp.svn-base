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

#include <io.hpp>

#include "transfer.hpp"
#include "../searches.hpp"
#include "../app.hpp"
#include "../traf.hpp"
#include "../verb.hpp"

using boost::bind;

namespace download_details {

Transfer::Transfer(SharedFile f):
	file_(f),
	fetcher_(new Fetcher(file_, validator_, sources_)),
	validator_(file_),
	traf_(app::traf_in()),
	connector_(new Connector(*fetcher_, sources_)),
	ticker_(app::io()),
	searcher_(app::searches())
	{
	}	

void Transfer::start(Handler h)
{
	file_.open();
	handler_ = h;

	SearchCriteria crt;
	crt.sha1 = file_->sha1();
	crt.ttr = file_->ttr();
	crt.md5 = file_->md5();
	crt.ed2k = file_->ed2k();

	searcher_.search(crt, bind(&Transfer::on_search_result, this, _1));
	//searcher_ = SearchMan::inst().create(crt, srch::AUTO);

	validator_.start();
	fetcher_->start();
	connector_->start();
	ticker_.tick(boost::posix_time::seconds(1), boost::bind(&Transfer::tick, this));
}

void Transfer::stop_work()
{
	searcher_.cancel();
//	SearchMan::inst().detach(searcher_);
	validator_.stop();
	fetcher_->stop();
	connector_->stop();
	ticker_.cancel();
	file_.close();
}

void Transfer::stop()
{
	stop_work();

	Handler h;
	h.swap(handler_);
}

void Transfer::done()
{
	stop_work();

	Handler h;
	h.swap(handler_);
	h();
}

int8 Transfer::priority() const { return fetcher_->priority(); }
void Transfer::priority(int8 n) { fetcher_->priority(n); }

std::vector<Downloader::Ptr> Transfer::dump_downloaders() const
{
	std::vector<Downloader::Ptr> v;

	BOOST_FOREACH(Downloader::Ptr p, fetcher_->downloaders())
		v.push_back(p);
	BOOST_FOREACH(Downloader::Ptr p, connector_->downloaders())
		v.push_back(p);

	return v;
}

void Transfer::accept(TcpSocketPtr sock, ip::Endpoint ep)
{
	Downloader::Ptr p(new Downloader(sock, ep, sources_));
	fetcher_->insert(p);
}

void Transfer::tick()
{
	if(is_idle())
	{
		done();
		return;
	}

	ticker_.tick(boost::posix_time::seconds(1), boost::bind(&Transfer::tick, this));
	
//	assert(searcher_.attached());
//	std::vector<SearchResult> results;
//	searcher_.dumpResults(std::back_inserter(results));
//	searcher_.clear();
//	BOOST_FOREACH(const SearchResult& r, results)
//		on_search_result(r);
}

void Transfer::on_search_result(const SearchResult& r)
{
	const SearchHit& h = r.hit;
	if(file_->name().empty()) file_->name(h.dn);
	file_->resolve_hashes(h.sha1, h.ttr, h.md5, h.ed2k);

	add_source(r.host);
}

std::size_t Transfer::rate() const
{
	std::size_t r = 0;

	BOOST_FOREACH(Downloader::Ptr p, fetcher_->downloaders())
		r += p->traf().rate();
	BOOST_FOREACH(Downloader::Ptr p, connector_->downloaders())
		r += p->traf().rate();

	return r;
}

void Transfer::add_source(const SourceHost& source)
{
	sources_[source.endpoint] = source;
}

bool Transfer::is_idle() const
{
#warning "optimize"
	return 
		!( 
			validator_.busy() || 
			fetcher_->active() || 
			connector_->active() || 
			( file_->map().status() != Integrity::FULL && file_->map().status() != Integrity::VALID ) 
		);
}


} // namespace download_details

