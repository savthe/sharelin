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

#include <stdexcept>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "default_reactor.hpp"
#include "appdefs.hpp"
#include "app.hpp"
#include "downloads.hpp"
#include "trafmanager.hpp"
#include "g2net.hpp"
#include "tools.hpp"
#include "comparers.hpp"
#include "history.hpp"
#include "filesystem.hpp"
#include "url.hpp"
#include "settings.hpp"
#include "share.hpp"
#include "download_recipe.hpp"
#include "verb.hpp"
#include "util.hpp"
#include "traf.hpp"
#include "ui_mgr.hpp"
#include "settings_defs.hpp"
#include "settings_control.hpp"
#include "uploads.hpp"

#include "srgroup.hpp"
#include "tag.hpp"

using namespace ui;
using boost::istarts_with;
using boost::lexical_cast;
using FileSystem::Unlink;
using util::now;
using util::since;

DefaultReactor::DefaultReactor(UiMgr& m):
	ui::Reactor(m)
{
	InitSortOption(ui::Tags::sres, ui::Tags::sources, false);
}

void DefaultReactor::ProcessQuery()
{
	switch(CurrentQuery().GetType())
	{
		case Tags::dls:
		{
			SelectTemplate("downloads");
			QDownloads();
			break;
		}
		case Tags::ups:
		{
			SelectTemplate("uploads");
			QUploads();
			break;
		}
		case Tags::srs:
		{
			SelectTemplate("searches");
			QSearches();
			break;
		}
		case Tags::incoming:
		{
			SelectTemplate("incoming");
			QIncoming();
			break;
		}
		case Tags::settings:
		{
			SelectTemplate("settings");
			QSettings();
			break;
		}
		case Tags::log:
		{
			SelectTemplate("log");
			QLog();
			break;
		}
		case Tags::share:
		{
			SelectTemplate("share");
			QShare();
			break;
		}
		case Tags::sr:
		{
			SelectTemplate("status");
			QNewSearch();
			break;
		}
		/*
		case Tags::srstop:
		{
			SelectTemplate("status");
			QStopSearch();
			break;
		}
		case Tags::srresume:
		{
			SelectTemplate("status");
			QResumeSearch();
			break;
		}
		*/
		case Tags::srdel:
		{
			SelectTemplate("status");
			QRemoveSearch();
			break;
		}
		case Tags::srshow:
		{
			SelectTemplate("results");
			QSearchResults();
			break;
		}
		case Tags::dlinfo:
		{
			SelectTemplate("dlinfo");
			QDownloadInfo();
			break;
		}
		case Tags::dlstop:
		{
			SelectTemplate("status");
			QStopDownload();
			break;
		}
		case Tags::dlresume:
		{
			SelectTemplate("status");
			QResumeDownload();
			break;
		}
		case Tags::dldel:
		{
			SelectTemplate("status");
			QRemoveDownload();
			break;
		}
		case Tags::dlpri:
		{
			SelectTemplate("status");
			QDownloadPriority();
			break;
		}
		case Tags::dl:
		{
			SelectTemplate("status");
			QDownloadCreate();
			break;
		}
		case Tags::status:
		{
			SelectTemplate("appstatus");
			QStatus();
			break;
		}
		case Tags::set:
		{
			SelectTemplate("status");
			QSet();
			break;
		}
		case Tags::dlhist:
		{
			SelectTemplate("dlhist");
			QDownloadsHistory();
			break;
		}
		case Tags::uplhist:
		{
			SelectTemplate("uplhist");
			QUploadsHistory();
			break;
		}
		case Tags::unlink:
		{
			SelectTemplate("status");
			QUnlink();
			break;
		}
		case Tags::sync:
		{
			SelectTemplate("status");
			QSyncShare();
			break;
		}
		case Tags::kill:
		{
			app::shutdown();
			break;
		}
		case Tags::uiset:
		{
			SelectTemplate("uiset");
			QUiset();
			break;
		}
		case Tags::debug:
		{
			SelectTemplate("status");
			app::debug();
			break;
		}
		default: throw std::runtime_error("Handler not found");
	};
}

void DefaultReactor::QUiset()
{
	for(ui::Query::Iterator i = CurrentQuery().Begin(); i != CurrentQuery().End(); ++i)
		SetOption(i->first, i->second);
}

void DefaultReactor::QSyncShare()
{
	app::sync_share();
	Put("status", "Share synchronization has been requested");
}

void DefaultReactor::QUnlink()
{
	const std::string& name = CurrentQuery().Get<std::string>(Tags::name);
	if(name.find("..") != std::string::npos || name.find("/") != std::string::npos)
		throw std::runtime_error("Restricted char sequence");
	Unlink(app::settings().general.complete + "/" + name);
	Put("status", "done");
}

void DefaultReactor::QSet()
{
	for(ui::Query::Iterator i = CurrentQuery().Begin(); i != CurrentQuery().End(); ++i)
		app::set_value(i->first, i->second);
	app::update_settings();
	Put("status", "done");
}

void DefaultReactor::QDownloadCreate()
{
	const ui::Query& q = CurrentQuery();

	if(q.Has(Tags::link)) 
		QDownloadCreateFromLink();
	else if(q.Has(Tags::sr) && q.Has(Tags::id)) 
		QDownloadCreateFromSearch();
}

void DefaultReactor::QDownloadCreateFromLink()
{
	const std::string link = Url::decode(CurrentQuery().Get<std::string>(ui::Tags::link));
	if(istarts_with(link, "ed2k://")) 
		QDownloadCreateFromEd2k(link);
	else if(istarts_with(link, "magnet:?")) 
		QDownloadCreateFromMagnet(link);
	else Put("status", "Link is not supported");
}

void DefaultReactor::QDownloadCreateFromMagnet(const std::string& link)
{
	std::stringstream s(link.substr(8));
	std::string line;
	DownloadRecipe req;
	std::map<ip::Endpoint, std::string> sources;
	while(std::getline(s, line, '&'))
	{
		line = Url::decode(line);
		//std::cout << line << std::endl;
		if(istarts_with(line, "xt=urn:ed2k:")) 
			req.ed2k.fromString(line.substr(12));
		else if(istarts_with(line, "xt=urn:ed2khash:")) 
			req.ed2k.fromString(line.substr(16));
		else if(istarts_with(line, "xt=urn:bitprint:"))
		{
			hash::BP bp;
			bp.fromString(line.substr(16)); 
			bp.split(req.sha1, req.ttr);
		}
		else if(istarts_with(line, "xt=urn:sha1:")) 
			req.sha1.fromString(line.substr(12));
		else if(istarts_with(line, "xt=urn:md5:")) 
			req.md5.fromString(line.substr(11));
		else if(istarts_with(line, "xl=")) 
			req.size = lexical_cast<file_offset_t>(line.substr(3));
		else if(istarts_with(line, "dn=")) req.name = line.substr(3);
		else if(istarts_with(line, "xs="))
		{
			try
			{
				std::size_t pos = line.find("http://");
				//fixme what if there is no http://
				line.erase(0, pos + 7);
				pos = line.find("/");
				const std::string ep = line.substr(0, pos);
				const std::string uri = line.substr(pos);
				ip::Endpoint endpoint;
				endpoint.fromString(ep);
				sources[endpoint] = uri;
			}
			catch(std::exception& e)
			{
				verb1::ui << "Couldn't parse magnet xs tag: " << e.what() << std::endl;
			}
		}
	}

	if(req.sha1.empty() && req.ed2k.empty() && req.ttr.empty() && req.md5.empty()) 
		throw std::runtime_error("Link has no hash");

	for(std::map<ip::Endpoint, std::string>::iterator i = sources.begin(); i != sources.end(); ++i)
	{
		SourceHost src;
		src.endpoint = i->first;
		src.uri = i->second;
		req.sources.push_back(src);
	}

	try { app::downloads().create(req); }
	catch (std::exception& err)
	{
		Put("status", "Can't create download. Reason: " + std::string(err.what()));
		return; 
	}

	Put("status", "Download created");
}


void DefaultReactor::QDownloadCreateFromEd2k(const std::string& link)
{
	std::stringstream s(link);
	
	std::string part;
	while(std::getline(s, part, '|') && part != "file");

	DownloadRecipe req;

	if(!(std::getline(s, req.name, '|'))) 
		throw std::runtime_error("Can't parse file name");

	std::getline(s, part, '|'); 
	req.size = lexical_cast<file_offset_t>(part);

	std::getline(s, part, '|');
	req.ed2k.fromString(part);

	try { app::downloads().create(req); }
	catch (std::exception& err)
	{
		Put("status", "Can't create download. Reason: " + std::string(err.what()));
		return; 
	}
	
	Put("status", "Download created");
}

void DefaultReactor::QDownloadCreateFromSearch()
{
	const Id sid = CurrentQuery().Get<Id>(ui::Tags::sr);
	UserSearch::Ptr s = manager_.searches().get(sid);

	if (!s) 
	{
		Put("status", "Bad search");
		return;
	}

	const Id id = CurrentQuery().Get<Id>(ui::Tags::id);

	SearchResult r;
	try { r = s->result(id); }
	catch (std::runtime_error&)
	{
		Put("status", "Bad result");
		return;
	}

	DownloadRecipe req;
	req.sha1 = r.hit.sha1;
	req.ttr = r.hit.ttr;
	req.md5 = r.hit.md5;
	req.ed2k = r.hit.ed2k;
	req.size = r.hit.size;
	req.name = r.hit.dn;

	for(std::size_t i = 0; i < s->results().size(); ++i)
	{
		const SearchResult& r = s->results().at(i);

		const SearchHit& tmp = r.hit;
		if(
			(!tmp.sha1.empty() && tmp.sha1 == req.sha1) || 
			(!tmp.ttr.empty() && tmp.ttr == req.ttr) || 
			(!tmp.md5.empty() && tmp.md5 == req.md5) || 
			(!tmp.ed2k.empty() && tmp.ed2k == req.ed2k)
		  )
		{
			req.sources.push_back(r.host);
		}
	}

	try { app::downloads().create(req); }
	catch (std::exception& err)
	{
		Put("status", "Can't create download. Reason: " + std::string(err.what()));
		return; 
	}

	Put("status", "Download created");
}

void DefaultReactor::QDownloadPriority()
{
	app::downloads().set_priority(CurrentQuery().Get<Id>(ui::Tags::id), CurrentQuery().Get<int>(ui::Tags::priority));
//	app::downloads().Update();
	Put("status", "done");
}

void DefaultReactor::QStopDownload()
{
	app::downloads().pause(CurrentQuery().Get<Id>(ui::Tags::id));
	Put("status", "done");
}

void DefaultReactor::QResumeDownload()
{
	app::downloads().resume(CurrentQuery().Get<Id>(ui::Tags::id));
	Put("status", "done");
}

void DefaultReactor::QRemoveDownload()
{
	app::downloads().cancel(CurrentQuery().Get<Id>(ui::Tags::id));
	Put("status", "done");
}

void DefaultReactor::QDownloadInfo()
{
	inf::Download inf(app::downloads().info_download(CurrentQuery().Get<Id>(ui::Tags::id)));
//	share::FileInfo inf = *pdl->GetFileInfo();
	
	Put("id", inf.f.partial);
	Put("name", inf.f.title());
	Put("path", inf.f.path);
	Put("size", inf.f.size);
	Put("hrsize", ui::HumanReadable(inf.f.size));
	Put("progress", ui::DownloadProgress(inf.f.map));
	Put("downloaders", inf.downloaders.size());
	Put("rate", ui::HumanReadable(inf.rate));
	Put("sha1", inf.f.sha1.empty() ? "n/a" : inf.f.sha1.toString());
	Put("ttr", inf.f.ttr.empty() ? "n/a" : inf.f.ttr.toString());
	Put("md5", inf.f.md5.empty() ? "n/a" : inf.f.md5.toString());
	Put("ed2k", inf.f.ed2k.empty() ? "n/a" : inf.f.ed2k.toString());
	Put("bp", (inf.f.sha1.empty() || inf.f.ttr.empty()) ? "n/a" : inf.f.sha1.toString() + "." + inf.f.ttr.toString());
	Put("tiger_min", inf.f.tiger_tree.empty() ? "n/a" : boost::lexical_cast<std::string>(inf.f.tiger_tree.block_size()));
	

//	std::vector<fio::Fragment> ranges;
	for(FileMap::iterator i = inf.f.map.begin(); i != inf.f.map.end(); ++i)
	{
		uint percent = 100 * double(double(i->length()) / inf.f.size);
		Put("chunk_size", percent);
		switch(i->color().value())
		{
			case Integrity::EMPTY: Put("chunk_status", "empty"); break;
			case Integrity::FULL: Put("chunk_status", "full"); break;
			case Integrity::VALID: Put("chunk_status", "valid"); break;
			case Integrity::INVALID: Put("chunk_status", "invalid"); break;
			//case Integrity::WRITE: Put("chunk_status", "active"); break;
			default: Put("chunk_status", "mixed");
		}
	}

	std::map<Integrity::Color, std::pair<uint, file_offset_t> > table;
	for(FileMap::iterator i = inf.f.map.begin(); i != inf.f.map.end(); ++i)
	{
		table[i->color().value()].first++;
		table[i->color().value()].second += i->length();
	}

	Put("parts_empty", table[Integrity::EMPTY].first);
	Put("parts_empty_p", ui::Progress(table[Integrity::EMPTY].second, inf.f.size));
	Put("parts_full", table[Integrity::FULL].first);
	Put("parts_full_p", ui::Progress(table[Integrity::FULL].second, inf.f.size));
	Put("parts_valid", table[Integrity::VALID].first);
	Put("parts_valid_p", ui::Progress(table[Integrity::VALID].second, inf.f.size));
	Put("parts_invalid", table[Integrity::INVALID].first);
	Put("parts_invalid_p", ui::Progress(table[Integrity::INVALID].second, inf.f.size));
//	Put("parts_active", table[Integrity::WRITE].first);
//	Put("parts_active_p", ui::Progress(table[Integrity::WRITE].second, inf.size));

	DumpDownloaders(*CurrentTemplate(), inf);

	/*
	std::vector<SourceHostEx> sources;
	pdl->CopySources(std::back_inserter(sources));
	uint nsource = 0;
	BOOST_FOREACH(const SourceHostEx& host, sources)
	{
		++nsource;
		Put("source_number", nsource);
		Put("source_endpoint", host.endpoint.toString());
		Put("source_connections", host.connections);
		Put("source_max", host.maxConnections);
		const std::time_t after = 
			app::now() < host.connectAfter ? 
			host.connectAfter - app::now() : 0;
		Put("source_after", after);
		Put("source_nick", Url::encode(host.nick));
	}
	*/
}

void DefaultReactor::QSearchResults()
{
	Id id = CurrentQuery().Get<Id>(ui::Tags::id);
	UserSearch::Ptr s = manager_.searches().get(id);

	//std::sort(results.begin(), results.end(), ui::ResultsComparer());
	const SearchCriteria& criteria = s->criteria();
	//Put("endless", criteria.endless);
	Put("dn", NameFromCriteria(criteria));
	Put("id", id);

	if (criteria.minSize) Put("min", ui::HumanReadable(criteria.minSize));
	if (criteria.maxSize) Put("max", ui::HumanReadable(criteria.maxSize));

	Put("extensions", ui::FormatExtensions(criteria.extensions.begin(), 
			criteria.extensions.end()));

	//typedef std::vector<SearchResult> ResultsContainer;
	//ResultsContainer results;
	//s.dumpResults(std::back_inserter(results));
	std::vector<ui::SearchResultGroup> groups;
	ui::CreateOrderedSRGroups(s->results().begin(), s->results().end(), std::back_inserter(groups));
	const SortOption sopt = GetSortOption(ui::Tags::sres);
	std::sort(groups.begin(), groups.end(), ui::SRGSortPredicate(sopt.SortBy()));
	if(!sopt.Forward()) std::reverse(groups.begin(), groups.end());
	//ui::SRGSort(groups.begin(), groups.end(), GetSortOption(ui::Tags::sres));

	WritePage(
			groups.begin(), 
			groups.end(), 
			ui::SearchResultWriter(CurrentTemplate(), GetTemplate("resultsgroup"), id)
			);
}

void DefaultReactor::QRemoveSearch()
{
	const Id id = CurrentQuery().Get<Id>(ui::Tags::id);
	UserSearch::Ptr s = manager_.searches().get(id);
	manager_.searches().remove(s);
	Put("status", "done");
}

void DefaultReactor::QNewSearch()
{
	SearchCriteria criteria;

	const ui::Query& q = CurrentQuery();

	const std::string dn = q.Get<std::string>(ui::Tags::dn);
	if(istarts_with(dn, "urn:sha1:")) criteria.sha1.fromString(dn.substr(9));
	else if(istarts_with(dn, "urn:ttr:")) criteria.ttr.fromString(dn.substr(8));
	else if(istarts_with(dn, "urn:ed2k:")) criteria.ed2k.fromString(dn.substr(9));
	else if(istarts_with(dn, "urn:md5:")) criteria.md5.fromString(dn.substr(8));
	else criteria.dn = dn;

	if(q.Has(ui::Tags::min))
		criteria.minSize = ui::GetSizeValue(q.Get<std::string>(ui::Tags::min));
	if(q.Has(ui::Tags::max))
		criteria.maxSize = ui::GetSizeValue(q.Get<std::string>(ui::Tags::max));
	if(q.Has(ui::Tags::media))
	{
		if(q.Get<std::string>(ui::Tags::media) != "any")
			criteria.mediaType = StaticName::fromString(q.Get<std::string>(ui::Tags::media));
	}
	if(q.Has(ui::Tags::ext))
	{
		std::stringstream s(q.Get<std::string>(ui::Tags::ext));
		std::string ext;
		while(s >> ext) criteria.extensions.insert(boost::to_lower_copy(ext));
	}

	try 
	{
		manager_.searches().create(criteria);
		Put("status", "New search created");
	}
	catch (std::exception& e)
	{
		Put("status", e.what());
	}
}

void DefaultReactor::QStatus()
{
	Put("full_name", app::fullname);
	Put("total_downloads", app::downloads().info().downloads.size());
	Put("total_uploads", app::uploads().count_uploads());
	Put("total_searches", manager_.searches().count());
	Put("hr_avr_bw_in", ui::HumanReadable(app::traf_in().rate()));
	Put("hr_avr_bw_out", ui::HumanReadable(app::traf_out().rate()));
	Put("address", (app::endpoint().address.toString()));
	Put("firewalled", (app::firewalled() ? "yes" : "no"));
	Put("uptime", ui::TimeRange(now() - app::started()));
}

void DefaultReactor::QVersion()
{
	using namespace inf::history;

	RecentVersion rv = app::history().recent_version();

	if(rv.birthday > app::birthday)
	{
		Put("version", rv.tag);
		Put("vlink", rv.link);
		std::string comments = rv.comments;
		boost::replace_all(comments, "\n", "<br>");
		Put("vcomments", comments);
	}
}

void DefaultReactor::QDownloadsHistory()
{
	using namespace inf::history;

	if(CurrentQuery().Has(ui::Tags::purge)) app::history().ClearDownloads();

	Put("dlhist_total_files", app::history().GetDownloads().size());
	BOOST_FOREACH(const Download& d, app::history().GetDownloads())
	{
		Put("dlhist_name", ui::AdaptString(FileSystem::FileName(d.path)));
		Put("dlhist_type", ui::FileType(d.path));
		Put("dlhist_size", ui::HumanReadable(d.size));
	}
}

void DefaultReactor::QUploadsHistory()
{
	using namespace inf::history;

	if(CurrentQuery().Has(ui::Tags::purge)) app::history().ClearUploads();

	Put("uplhist_total_files", app::history().GetUploads().size());
	file_offset_t sz = 0;
	BOOST_FOREACH(const Upload& u, app::history().GetUploads())
	{
		sz += u.transferred;

		Put("uplhist_name", ui::AdaptString(FileSystem::FileName(u.object)));
		Put("uplhist_size", ui::HumanReadable(u.transferred));
		Put("uplhist_address", u.endpoint.address.toString());
		Put("uplhist_nick", Url::decode(u.nick));
		Put("uplhist_type", ui::FileType(u.object));
		Put("uplhist_client", ui::AdaptString(u.client));
	}

	Put("uplhist_total_size", ui::HumanReadable(sz));
}

void DefaultReactor::QDownloads()
{
	using namespace download_details::dl_state;

	ui::Template tplDlrs = GetTemplate("downloaders");

	inf::DownloadManager minf(app::downloads().info());
	std::vector<inf::Download> dls;
	for(uint i = 0; i < minf.downloads.size(); ++i)
		dls.push_back(app::downloads().info_download(minf.downloads[i]));

	ui::Comparers::Download comparer(GetSortOption(ui::Tags::adl));
	std::sort(dls.begin(), dls.end(), comparer);

	Put("total_downloads", dls.size());
	for(uint i = 0; i < dls.size(); ++i)
	{
		const inf::Download& inf = dls[i];

		Put("download_name", ui::AdaptString(inf.f.title()));
		Put("download_size", ui::HumanReadable(inf.f.size));
		Put("download_rate", ui::HumanReadable(inf.rate));
		Put("sources", inf.sources_count);
		Put("active_sources", inf.downloaders.size());
		Put("progress", ui::DownloadProgress(inf.f.map));
		Put("download_id", inf.f.partial);
		Put("priority", int(inf.priority));
		Put("dl_paused", inf.state == PAUSE);

		switch(inf.state)
		{
			case TRANSFER: Put("dl_status", "transfer"); break; 
			case HASH: Put("dl_status", "hash"); break; 
			case MOVE: Put("dl_status", "move"); break; 
			case PAUSE: Put("dl_status", "paused"); break; 
			case STOP: Put("dl_status", "queued"); break; 
			case ERROR: Put("dl_status", "error"); break; 
			default: assert(!"can't be here");
		}

		
//		if( !Details::ExpandDownloads() ) 
//		{
//			Put("downloaders", "");
//			continue;
//		}
	

		tplDlrs.ClearValues();
		DumpDownloaders(tplDlrs, inf);

		Put("downloaders", tplDlrs);
	}
}

void DefaultReactor::DumpDownloaders(ui::Template& tpl, const inf::Download& download)
{
	using namespace download_details::dlr_state;

	BOOST_FOREACH(const inf::Downloader& info, download.downloaders)
	{
		tpl.Put("dlr_address", info.endpoint.toString());
		tpl.Put("dlr_pushed", info.pushed);
		
		switch (info.state) 
		{
			case IDLE: tpl.Put("dlr_status", "idle"); break;
			case CONNECT: tpl.Put("dlr_status", "connect"); break;
			case TRANSFER: tpl.Put("dlr_status", "file"); break;
			case CHAT: tpl.Put("dlr_status", "talking"); break;
			case QUEUED: tpl.Put("dlr_status", "queued"); break;
			case CLOSED: tpl.Put("dlr_status", "closed"); break;
		};
//			case Downloader::TRANSFER: tpl.Put("dlr_status", loader::FETCH_FILE ? "file" : "tiger"); break;
			
		tpl.Put("dlr_left", ui::RankedNumber(info.range.first(), SpaceString()));
		tpl.Put("dlr_right", ui::RankedNumber(info.range.last(), SpaceString()));
		tpl.Put("dlr_length", ui::RankedNumber(info.range.length(), SpaceString()));
		tpl.Put("dlr_total", ui::HumanReadable(info.total));
		tpl.Put("dlr_qnum", info.q_pos);
		tpl.Put("dlr_qsize", info.q_length);
		tpl.Put("dlr_rate", ui::HumanReadable(info.rate));
	}
}

void DefaultReactor::QUploads()
{
	if(CurrentQuery().Has(ui::Tags::purge)) app::history().ClearUploads();

	inf::UploadManager m = app::uploads().info();
	Put("total_uploads", m.uploaders.size());
	for(size_t i = 0; i < m.uploaders.size(); ++i)
	{
		const inf::Uploader& u = m.uploaders[i];

		if(!u.queued)
		{
			Put("a_name", ui::AdaptString(u.title));
			Put("a_size", ui::HumanReadable(u.sent));
			Put("a_rate", u.rate / 1024);
			Put("a_address", u.endpoint.address.toString());
			Put("a_nick", Url::decode(u.nick));
			Put("a_type", ui::FileType(u.title));
			Put("a_client", ui::AdaptString(u.client));
		}
		else
		{
			Put("q_name", ui::AdaptString(u.title));
			Put("q_address", u.endpoint.address.toString());
			Put("q_nick", Url::decode(u.nick));
			Put("q_type", ui::FileType(u.title));
			Put("q_client", ui::AdaptString(u.client));
		}
	}
}

void DefaultReactor::QIncoming()
{
	std::vector<std::string> paths;

	FileSystem::ScanDirectory(app::settings().general.complete, std::back_inserter(paths), FileSystem::REGULAR_FILE, FileSystem::NOT_FOLLOW);

	unsigned n = 0;
	BOOST_FOREACH(const std::string& s, paths)
	{
		++n;
		Put("name", ui::AdaptString(FileSystem::FileName(s)));
		Put("size", ui::HumanReadable(FileSystem::FileSize(s)));
		Put("type", ui::FileType(s));
	}

	Put("total_files", n);
}

void DefaultReactor::QSettings()
{
	using namespace settings_details::Options;
	const SettingsStorage* p = &app::settings();
	Put(std::string(userName), p->general.userName);
	Put(webuiPort, p->webui.port);
	Put(telnetPort, p->telnet.port);
	Put(webuiPageSize, p->webui.pageSize);
	Put(telnetPageSize, p->telnet.pageSize);
	
	std::string s;
	BOOST_FOREACH(IpSegment seg, p->general.uisegments)
	{
		s += seg.first().toString();
		if(seg.first() != seg.last())
			s += "-" + seg.last().toString();
		s += "; ";
	}
	Put(remoteRanges, s);

	Put(webuiStyle, p->webui.style);
	Put(telnetStyle, p->telnet.style);
	Put(listenPort, p->net.listenPort);
	switch(p->net.firewalled)
	{
		case settings_types::firewalled::YES: s = "yes"; break;
		case settings_types::firewalled::NO: s = "no"; break;
		default: s = "auto"; break;
	}
	Put(firewalled, s);
	Put("max_bw_in_kb", p->net.maxBwIn / 1024);
	Put("max_bw_out_kb", p->net.maxBwOut / 1024);
	
	s.clear();
	BOOST_FOREACH(const std::string& path, p->share.folders)
		s += path + "; ";
	Put(settings_details::Options::share, s);
	
	Put(complete, p->general.complete);
	Put(incomplete, p->general.incomplete);
	Put(maxHubs, p->net.maxHubs);
	Put(uploadsPerHost, p->net.uploadsPerHost);
	Put(maxUploads, p->net.maxUploads);
	Put(queueSize, p->net.queueSize);
	Put(maxDownloads, p->net.maxDownloads);
	Put(maxFiles, p->net.maxFiles);
	Put(hashRate, p->share.hashRate);
}

void DefaultReactor::QLog()
{
	const unsigned int MAX = CurrentQuery().Has(Tags::ps) ? CurrentQuery().Get<unsigned int>(Tags::ps) : PageSize();
	const std::string log = app::home() + "/sharelin.log";

	std::ifstream f(log.c_str());
	if(!f.good()) throw std::runtime_error("Couldn't open log file: " + log);

	std::string s;
	std::list<std::string> lines;
	while(std::getline(f, s))
	{
		lines.push_back(s);
		if(lines.size() > MAX) lines.pop_front();
	}

	std::reverse(lines.begin(), lines.end());

	BOOST_FOREACH(const std::string& msg, lines)
	{
		std::stringstream ss(msg);
		int level;
		std::string timestamp;
		std::string message;
		ss >> level >> timestamp;
		std::getline(ss, message);
		Put("level", level);
		Put("line", timestamp + " " + message);
	}
}


void DefaultReactor::QShare()
{
	std::vector<SharedFile> c;

	app::share().dump(c);

	std::vector<const FileInfo*> v;
	BOOST_FOREACH(const SharedFile& f, c)
		v.push_back(&(f->info()));

	WritePage(
			v.begin(), 
			v.end(), 
			ui::ShareWriter(CurrentTemplate()), 
			ui::Comparers::SharedFiles(GetSortOption(ui::Tags::sfl))
			);

	/*
	WritePageInfo(page, pageSize, v.size());
	ui::Comparers::Files comparer(GetSortOption(ui::Tags::sfl));
	WritePage(v.begin(), v.end(), page, pageSize, ui::ShareWriter(*CurrentTemplate()));
	*/
}

void DefaultReactor::QSearches()
{
	Put("total_searches", manager_.searches().count());

	for (UiSearches::iterator i = manager_.searches().begin(); i != manager_.searches().end(); ++i)
	{
		UserSearch::Ptr s = *i;

		const SearchCriteria& crit = s->criteria();
		Put("id", s->id());
		std::stringstream scrit;
		scrit << ui::NameFromCriteria(crit);
		if(crit.minSize) scrit << ", Min: " << ui::HumanReadable(crit.minSize);
		if(crit.maxSize) scrit << ", Max: " << ui::HumanReadable(crit.maxSize);
		Put("criteria", scrit.str());

		std::string exts;
		BOOST_FOREACH(const std::string& str, crit.extensions) exts += str + " ";
		Put("extensions", exts);

		Put("hits", s->results().size());
	}
}


void DefaultReactor::QNeighbors()
{
	int counter = 0;

	g2::Endpoints neighbors = app::g2net().getNeighbors();
	BOOST_FOREACH(ip::Endpoint e, neighbors)
	{
		const g2::NodeInfoEx h = app::g2net().getNeighborInfo(e.address);
		Put("h_num", ++counter);
		Put("h_address", h.info.endpoint.toString());
		Put("h_good_packets", h.stats.goodPackets);
		Put("h_bad_packets", h.stats.badPackets);
		Put("h_sent_packets", h.stats.sentPackets);
		Put("h_agent", h.info.agent.empty() ? "Unknown" : h.info.agent);
		Put("h_connected_time", ui::TimeRange(now() - h.stats.startTime));
	}

	g2::Endpoints connectors = app::g2net().getConnectors();
	counter = 0;
	BOOST_FOREACH(ip::Endpoint e, connectors)
	{
		const g2::ConnectorInfo h = app::g2net().getConnectorInfo(e.address);
		Put("hs_num", ++counter);
		Put("hs_address", e.toString());
		Put("hs_agent", h.agent.empty() ? "Unknown" : h.agent);
		if(h.connecting) Put("hs_connection_status", "connecting");
		else Put("hs_connection_status", "handshaking");
	}
}


