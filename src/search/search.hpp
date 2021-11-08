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

#ifndef SEARCH_SEARCH_HPP
#define SEARCH_SEARCH_HPP

#include "idgen.hpp"
#include "holder.hpp"
#include "queue.hpp"
#include "period.hpp"
#include "../g2packet.hpp"
#include "searcher_impl.hpp"

namespace search_details {

class KeyKeeper;
class KeyGather;

class Search
{
public:
	Search(KeyKeeper&, KeyGather&);

	void attach(SearcherImpl*);
	void detach(SearcherImpl*);
	void tick();
	void onQH2(const g2::Packet&, ip::Endpoint);
	void onQA(const g2::Packet&, ip::Endpoint, ip::protocol::Type);
	void detach_all();

	void debug() const;

private:
	enum { HUB_REST_TIME = 60 };
	bool localSearch(SearcherImpl*);
	bool foreignSearch(SearcherImpl*);

	KeyKeeper& keeper_;
	KeyGather& gather_;

	Misc::IdGenerator idgen_;
	Holder holder_;
	Queue queue_;
	util::Period tick_;
};	

} // namespace search_details
#endif // SEARCH_SEARCH_HPP

