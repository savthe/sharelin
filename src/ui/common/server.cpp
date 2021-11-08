/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011  Andrey Stroganov <savthe@gmail.com>

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

#include "server.hpp"
#include "app.hpp"
#include "settings.hpp"
#include "tag.hpp"
#include "filesystem.hpp"
#include "reactor.hpp"
#include "appdefs.hpp"
#include "verb.hpp"
#include "ui_mgr.hpp"

using FileSystem::Exists;

namespace {

std::string FindStyle(const std::string& style, const std::string& parent)
{
	const std::string path = parent + "/" + style;
	std::set<std::string> paths;
	paths.insert(app::home());
	paths.insert("/usr/local/share/sharelin");
	paths.insert("/usr/share/sharelin");
#ifdef DEFAULT_ROOT
	paths.insert(std::string(DEFAULT_ROOT) + "/share/sharelin");
#endif

	std::string root;

	for(std::set<std::string>::iterator i = paths.begin(); i != paths.end(); ++i)
	{
		const std::string s = *i + "/" + path;
		verb1::ui << "Searching style '" << style << "' in " << s << " => ";
		if(Exists(s))
		{
			verb1::ui << "Found" << std::endl;
			root = s;
			break;
		}
		else verb1::ui << "Not found" << std::endl;
	}

	return root;
}

} //namespace 

namespace ui {
namespace base {

Server::Server(ba::io_service& io, Reactor* pReactor):
	acceptor_(io, ba::ip::tcp::v4()),
	pReactor_(pReactor)
{
}

Server::~Server()
{
	delete pReactor_;
}

void Server::ProcessQuery(const Query& q, std::string& reply)
{
	pReactor_->ProcessQuery(q, reply);
}

void Server::Start(uint port)
{
	ba::socket_base::reuse_address reuse(true);
	acceptor_.set_option(reuse);
	acceptor_.bind(ba::ip::tcp::endpoint(ba::ip::tcp::v4(), port));
	acceptor_.listen();
	ClientPtr pNewClient(CreateClient());
	acceptor_.async_accept(pNewClient->Socket(), boost::bind(&Server::Accept, this, pNewClient, ba::placeholders::error));
}

void Server::Detach(ClientPtr p)
{
	clients_.erase(p);
}

void Server::stop()
{
	acceptor_.cancel();

	BOOST_FOREACH(ClientPtr p, clients_)
		p->stop();

	clients_.clear();
}

void Server::Accept(ClientPtr p, const bs::error_code& err)
{
	if(err != ba::error::operation_aborted)

	{
		/*
		Ip::Address addr = p->GetEndpoint().address;

		bool found = false;
		BOOST_FOREACH(IpSegment s, System::GetSettings()->general.uisegments)
			found |= s.Contains(addr);

		if(found)
		{
			clients_.insert(p);
			p->Start();
		}
		else
		{
			logger::bas << "Dropping webui client connection " << addr << " (Ip not allowed)" << std::endl;
		}
		*/

		clients_.insert(p);
		p->Start();
		ClientPtr pNewClient(CreateClient());
		acceptor_.async_accept(pNewClient->Socket(), boost::bind(&Server::Accept, this, pNewClient, ba::placeholders::error));
	}
}

/*
void Server::UpdateSortOption(Tags::Type key, Tags::Type sortby)
{
	sortOptions_[key].Update(sortby);
}

SortOption GetSortOption(Tags::Type key) const
{
	SortOptionContainer::const_iterator i = sortOptions_.find(key);
	if(i != sortOptions_.end()) return i->second;
	return SortOption();
}
*/

void Server::Style(const std::string& style)
{
	if(style == style_) return;
	std::string found = FindStyle(style, StylePrefix());
	if(found.empty())
	{
		verb1::ui << "Style not found. Looking for default style" << std::endl;
		found = FindStyle("default", StylePrefix());
	}
	if(found.empty()) throw std::runtime_error("Style not found. Help!");

	style_ = style;
	styleRoot_ = found;
	pReactor_->Root(styleRoot_);
}

std::string Server::StyleRoot() const
{
	return styleRoot_;
}

} // namespace base 
} // namespace ui

