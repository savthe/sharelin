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

#ifndef DL_TRANSFER_HPP
#define DL_TRANSFER_HPP

#include "fetcher.hpp"
#include "connector.hpp"
#include "sources.hpp"
#include "../sourcehost.hpp"
#include "guid.hpp"
#include "../traf_client.hpp"
#include "validator.hpp"
#include "../searcher.hpp"
#include "ticker.hpp"
#include "../sharedfile.hpp"

namespace download_details {

class Transfer
{
public:
	typedef boost::function<void()> Handler;

	Transfer(SharedFile);

	void start(Handler);
	void stop();
	void accept(TcpSocketPtr, ip::Endpoint);
	void add_source(const SourceHost&);
	std::size_t count_downloaders() const { return fetcher_->downloaders().size() + connector_->downloaders().size(); }
	std::size_t count_sources() const { return sources_.size(); }

	const Sources& sources() const { return sources_; }
	std::vector<Downloader::Ptr> dump_downloaders() const;
	std::size_t rate() const;

	int8 priority() const;
	void priority(int8);

private:
	bool is_idle() const;
	void tick();
	void done();
	void stop_work();
	void on_search_result(const SearchResult&);

	SharedFile file_;
	Fetcher::Ptr fetcher_;
	Validator validator_;
	TrafClient traf_;
	Sources sources_;
	boost::shared_ptr<Connector> connector_;
	Ticker ticker_;
	Handler handler_;
	Searcher searcher_;
};

} // namespace download_details

#endif // DL_TRANSFER_HPP

