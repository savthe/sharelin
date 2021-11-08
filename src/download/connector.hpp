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

#ifndef DL_CONNECTOR_HPP
#define DL_CONNECTOR_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

#include "sources.hpp"
#include "fetcher.hpp"
#include "downloaders.hpp"
#include "ticker.hpp"

namespace download_details {

class Connector: public boost::enable_shared_from_this<Connector>, public boost::noncopyable
{
public:
	Connector(Fetcher&, Sources&);

	void start();
	void stop();
	bool active() const { return !downloaders_.empty(); }
	const Downloaders& downloaders() const { return downloaders_; }

private:
	//void Push(ip::Endpoint);
	void on_connected(const bs::error_code&, Downloader::Ptr);
	void update_cache();
	ip::Endpoint select_endpoint();
	void push(ip::Endpoint);
	void tick();
	void make_connections();

	Downloaders downloaders_;
	Fetcher& fetcher_;
	Sources& sources_;
	std::vector<ip::Endpoint> cache_;
	Ticker ticker_;
};

} // namespace download_details

#endif // DL_CONNECTOR_HPP

