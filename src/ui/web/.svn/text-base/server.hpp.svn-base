/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov <savthe@gmail.com>

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

#ifndef WEBUISERVER_HPP
#define WEBUISERVER_HPP

#include <map>

#include "../common/server.hpp"
#include "../common/tag.hpp"

class UiMgr;

namespace ui {
namespace web {

class Server: public base::Server
{
public:
	Server(UiMgr&);
	~Server();

//	void SetReloadRate(Ui::Tags::Type, uint);
//	uint GetReloadRate(Ui::Tags::Type) const;
//	void ExpandDownloads(bool);
//	bool ExpandDownloads() const;
	void UpdateSettings();
	virtual void Start();
	void debug() const;

private:
	base::Server::ClientPtr CreateClient();
	std::string StylePrefix() const;
//	std::map<Ui::Tags::Type, uint> reloadRates_;
//	bool expandDownloads_;
//	WebuiReactor* pReactor_;
};

} // namespace web
} // namespace ui

#endif //WEBUISERVER_HPP

