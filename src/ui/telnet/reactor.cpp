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

#include "reactor.hpp"
#include "settings.hpp"
#include "ui_mgr.hpp"

namespace ui {
namespace telnet {

Reactor::Reactor(UiMgr& m):
	DefaultReactor(m)
{
}


uint Reactor::PageSize() const
{
	const uint n = app::settings().telnet.pageSize;
	return n ? n : 20;
}

void Reactor::ProcessQuery()
{
	using namespace ui;

//	for(Ui::Command::Iterator i = CurrentQuery().Begin(); i != CurrentQuery().End(); ++i)
//		std::cout << i->first << " : " << i->second << std::endl;

	if(CurrentQuery().GetType() == Tags::r) CurrentQuery(lastQuery_);
	else lastQuery_ = CurrentQuery();
	switch(CurrentQuery().GetType())
	{
		case Tags::nb:
		{
			SelectTemplate("nb");
			QNeighbors();
			break;
		}
		default: DefaultReactor::ProcessQuery();
	}
	/*
	const std::string strQuery = StaticName::ToString(CurrentQuery().Cmd());
	const std::string strReload = "webui_" + strQuery + "_reload";
	const std::string strExpandDownloads = "webui_expand_downloads";

	switch(CurrentQuery().Cmd())
	{
		case Tags::home:
		{
			SelectTemplate("home");
			QStatus();
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
		case Tags::srstop:
		{
			SelectTemplate("searches");
			QStopSearch();
			QSearches();
			break;
		}
		case Tags::srdel:
		{
			SelectTemplate("searches");
			QRemoveSearch();
			QSearches();
			break;
		}
		case Tags::srresume:
		{
			SelectTemplate("searches");
			QResumeSearch();
			QSearches();
			break;
		}
		case Tags::dlstop:
		{
			SelectTemplate("downloads");
			QStopDownload();
			QDownloads();
			break;
		}
		case Tags::dlresume:
		{
			SelectTemplate("downloads");
			QResumeDownload();
			QDownloads();
			break;
		}
		case Tags::dldel:
		{
			SelectTemplate("downloads");
			QRemoveDownload();
			QDownloads();
			break;
		}
		case Tags::dlpri:
		{
			SelectTemplate("downloads");
			QDownloadPriority();
			QDownloads();
			break;
		}
		case Tags::dls:
		{
			SelectTemplate("downloads");
			if(CurrentQuery().Has(Tags::expand))
				SetOption(strExpandDownloads, CurrentQuery().Get<uint>(Tags::expand));
			QDownloads();
			break;
		}
		case Tags::dlgroup:
		{
			SelectTemplate("downloads");
			QGroupAction();
			QDownloads();
			break;
		}
		case Tags::unlink:
		{
			SelectTemplate("incoming");
			QUnlink();
			QIncoming();
			break;
		}

		default: DefaultReactor::ProcessQuery();
	};
	
	Put("query", StaticName::ToString(CurrentQuery().Cmd()));

	if(CurrentQuery().Has(Tags::pgreload))
		SetOption(strReload, CurrentQuery().Get<uint>(Tags::pgreload));

	Put("webui_reload", GetOption(strReload).first);
	Put(strExpandDownloads, GetOption(strExpandDownloads).first);
	*/

}

} // namespace telnet
} // namespace ui

