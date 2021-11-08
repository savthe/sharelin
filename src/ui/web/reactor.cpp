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

#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "url.hpp"
#include "reactor.hpp"
#include "ui/common/query.hpp"
#include "settings.hpp"
#include "server.hpp"
#include "downloads.hpp"
#include "verb.hpp"
#include "ui_mgr.hpp"

using boost::istarts_with;
using boost::lexical_cast;
using boost::iequals;

namespace ui {
namespace web {

Reactor::Reactor(UiMgr& m):
	DefaultReactor(m)
{
}

void Reactor::ProcessQuery()
{
	const std::string strQuery = StaticName::toString(CurrentQuery().GetType());
	const std::string strReload = "webui_" + strQuery + "_reload";
	const std::string strExpandDownloads = "webui_expand_downloads";

//	for(Ui::Command::Iterator i = CurrentQuery().Begin(); i != CurrentQuery().End(); ++i)
//		std::cout << i->first << " : " << i->second << std::endl;
	switch(CurrentQuery().GetType())
	{
		case Tags::home:
		{
			SelectTemplate("home");
			QNeighbors();
			QVersion();
			break;
		}
		case Tags::sr:
		{
			SelectTemplate("searches");
			QNewSearch();
			QSearches();
			break;
		}
		/*
		case Tags::srstop:
		{
			SelectTemplate("searches");
			QStopSearch();
			QSearches();
			break;
		}
		*/
		case Tags::srdel:
		{
			SelectTemplate("searches");
			QRemoveSearch();
			QSearches();
			break;
		}
		/*
		case Tags::srresume:
		{
			SelectTemplate("searches");
			QResumeSearch();
			QSearches();
			break;
		}
		*/
		case Tags::dlstop:
		{
			SelectTemplate("downloads");
			QStopDownload();
			QDownloads();
			QDownloadsHistory();
			break;
		}
		case Tags::dlresume:
		{
			SelectTemplate("downloads");
			QResumeDownload();
			QDownloads();
			QDownloadsHistory();
			break;
		}
		case Tags::dldel:
		{
			SelectTemplate("downloads");
			QRemoveDownload();
			QDownloads();
			QDownloadsHistory();
			break;
		}
		case Tags::dlpri:
		{
			SelectTemplate("downloads");
			QDownloadPriority();
			QDownloads();
			QDownloadsHistory();
			break;
		}
		case Tags::ups:
		{
			SelectTemplate("uploads");
			QUploads();
			QUploadsHistory();
			break;
		}
		case Tags::dls:
		{
			SelectTemplate("downloads");
			if(CurrentQuery().Has(Tags::expand))
				SetOption(strExpandDownloads, CurrentQuery().Get<uint>(Tags::expand));
			QDownloads();
			QDownloadsHistory();
			break;
		}
		case Tags::dlgroup:
		{
			SelectTemplate("downloads");
			QGroupAction();
			QDownloads();
			QDownloadsHistory();
			break;
		}
		case Tags::unlink:
		{
			SelectTemplate("incoming");
			QUnlink();
			QIncoming();
			break;
		}
		case Tags::uiset:
		{
			SelectTemplate("uiset");
			QUiset();
			break;
		}

		default: DefaultReactor::ProcessQuery();
	};
	
	QStatus();

	Put("query", StaticName::toString(CurrentQuery().GetType()));
	Put(strExpandDownloads, GetOption(strExpandDownloads).first);
}

void Reactor::QUiset()
{
	for(Query::Iterator i = CurrentQuery().Begin(); i != CurrentQuery().End(); ++i)
	{
		if(!iequals(i->first, "redirect")) 
			SetOption(i->first, i->second);
	}

	const std::string& redirect = Url::decode(CurrentQuery().Get<std::string>(Tags::redirect));
	Put("redirect", redirect);
}

uint Reactor::PageSize() const
{
	const uint n = app::settings().webui.pageSize;
	return n ? n : 500;
}

void Reactor::QGroupAction() const
{
	const std::string q = CurrentQuery().Get<std::string>(Tags::selected);

	for(Query::Iterator i = CurrentQuery().Begin(); i != CurrentQuery().End(); ++i)
	{
		if(!istarts_with(i->first, "id")) continue;

		try
		{
			const Id id = lexical_cast<Id>(i->first.substr(2));
			//dl::DownloadPtr pdl = app::downloads().Get(id);

			if(iequals(q, "pause")) app::downloads().pause(id);
			else if(iequals(q, "resume")) app::downloads().resume(id);
			else if(iequals(q, "remove")) app::downloads().cancel(id);
			else if(iequals(q, "priorityl")) app::downloads().set_priority(id, 0);
			else if(iequals(q, "priorityb")) app::downloads().set_priority(id, 1);
			else if(iequals(q, "priorityn")) app::downloads().set_priority(id, 2);
			else if(iequals(q, "prioritya")) app::downloads().set_priority(id, 3);
			else if(iequals(q, "priorityh")) app::downloads().set_priority(id, 4);
		}
		catch(std::exception& e) 
		{ 
			verb2::ui << "Got exception in download group operation: " << e.what() << std::endl;
		}

	}
	
}

} // namespace web
} // namespace ui


