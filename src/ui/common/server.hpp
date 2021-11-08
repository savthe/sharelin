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

#ifndef UISERVER_HPP
#define UISERVER_HPP

#include <set>
#include <map>
#include <string>
#include <sstream>

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include "io.hpp"
#include "env.hpp"
#include "sortoption.hpp"
#include "tag.hpp"
#include "client.hpp"
#include "searches.hpp"

class UiMgr;

namespace ui {

class Reactor;
class Query;

namespace base {

class Server
{
public:
	typedef boost::shared_ptr<Client> ClientPtr;
	Server(ba::io_service&, Reactor*);
	virtual ~Server();
	void Start(uint port);
	virtual void stop();
	virtual void Detach(ClientPtr p);
	std::string StyleRoot() const;
	void Style(const std::string&);
	virtual void ProcessQuery(const Query&, std::string&);


/*
	void UpdateSortOption(Ui::Tags::Type key, Ui::Tags::Type sortby)
	{
		Ui::SortOption opt = sortOptions_[key];
		if(opt.SortBy() == sortby) opt.Forward(!opt.Forward());
		else opt = Ui::SortOption(sortby);
		sortOptions_[key] = opt;
	}

	Ui::SortOption GetSortOption(Ui::Tags::Type key) const
	{
		SortOptionContainer::const_iterator i = sortOptions_.find(key);
		if(i != sortOptions_.end()) return i->second;
		return Ui::SortOption();
	}
	*/

protected:
//	typedef std::map<Ui::Tags::Type, Ui::SortOption> SortOptionContainer;

	virtual void Accept(ClientPtr p, const bs::error_code& err);
	virtual std::string StylePrefix() const = 0;
	virtual ClientPtr CreateClient() = 0;

	std::set<ClientPtr> clients_;
	ba::ip::tcp::acceptor acceptor_;
	std::string style_;
	std::string styleRoot_;
	Reactor* pReactor_;
	//SortOptionContainer sortOptions_;
};

} // namespace base
} // namespace ui

#endif //UISERVER_HPP

