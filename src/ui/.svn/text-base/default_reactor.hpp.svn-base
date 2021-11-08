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

#ifndef DEFAULTREACTOR_HPP
#define DEFAULTREACTOR_HPP

#include <algorithm>

#include "reactor.hpp"
#include "template.hpp"
#include "tag.hpp"
#include "types.hpp"
#include "../../inf/download.hpp"

class UiMgr;

class DefaultReactor: public ui::Reactor
{
protected:
	DefaultReactor(UiMgr&);
//	virtual void Query(const Ui::Command&, std::string&);
	virtual void ProcessQuery();
	virtual void QNewSearch();
	virtual void QStatus();
	virtual void QNeighbors();
	virtual void QVersion();
	virtual void QDownloads();
	virtual void QUploads();
	virtual void QSearches();
	virtual void QIncoming();
	virtual void QSettings();
	virtual void QLog();
	virtual void QShare();
	virtual void QRemoveSearch();
//	virtual void QResumeSearch();
//	virtual void QStopSearch();
	virtual void QSearchResults();
	virtual void QDownloadInfo();
	virtual void QDownloadPriority();
	virtual void QStopDownload();
	virtual void QResumeDownload();
	virtual void QRemoveDownload();
	virtual void QDownloadCreate();
	virtual void QDownloadCreateFromLink();
	virtual void QDownloadCreateFromEd2k(const std::string&);
	virtual void QDownloadCreateFromMagnet(const std::string&);
	virtual void QDownloadCreateFromSearch();
	virtual void QSet();
	virtual void QUnlink();
	virtual void QDownloadsHistory();
	virtual void QUploadsHistory();
	virtual void QUiset();
	virtual void QSyncShare();

	void DumpDownloaders(ui::Template&, const inf::Download&);

	template <typename T, typename Writer>
	void WritePage(T begin, T end, Writer w)
	{
		const uint pageSize = CurrentQuery().Has(ui::Tags::ps) ? CurrentQuery().Get<uint>(ui::Tags::ps) : PageSize();
		const uint page = CurrentQuery().Has(ui::Tags::p) ? CurrentQuery().Get<uint>(ui::Tags::p) - 1 : 0;

		const uint size = std::distance(begin, end);
	//	pageSize = pageSize ? pageSize : size;
		const uint nstart = page * pageSize;
		const uint pagesCount = size % pageSize == 0 ? size / pageSize : size / pageSize + 1;
		if(nstart > size) return;
		std::advance(begin, nstart);
	
		w.Put("page_container_size", size);
		w.Put("page_current", page + 1);
		w.Put("page_total", pagesCount);
		for(uint i = 1; i <= pagesCount; ++i) w.Put("page_number", i);

		for(uint i = 0; i < pageSize && begin != end; ++i)
		{
			w(*begin++);
			w.Put("number", i + 1);
		}
	}

	template <typename T, typename Writer, typename Comparer>
	void WritePage(T begin, T end, Writer w, Comparer c)
	{
		std::sort(begin, end, c);
		WritePage(begin, end, w);
	}

};

/*

struct DefaultReactor
{
	typedef boost::shared_ptr<Search::Searcher> SearcherPtr;
	struct Unhandled: public std::runtime_error
	{
		Unhandled(const std::string& err): std::runtime_error(err) {}
	};

	static void React(const ui::Command& cmd, ui::Template& tpl)
	{
		typedef uiCategories<Details> Cat;

		switch(cmd.Cmd())
		{
			case ui::Tags::kill:
			{
				System::Shutdown();
				break;
			}
			case ui::Tags::dlnew:
			{

				if(cmd.Has(ui::Tags::link)) CreateDownloadFromLink(Url::Decode(cmd.Get<std::string>(ui::Tags::link)));
				else if(cmd.Has(ui::Tags::sr) && cmd.Has(ui::Tags::id))
					CreateDownloadFromSearch(cmd.Get<ID>(ui::Tags::sr), cmd.Get<ID>(ui::Tags::id));
				else throw Unhandled("No parameters");
			}
			case ui::Tags::dl:
			{
				DownloadManager* mgr = System::GetDownloadMgr();
				if(cmd.Has(ui::Tags::selected)) OnSelectedDownloads(cmd);
				else if(cmd.Has(ui::Tags::del)) mgr->Get(cmd.Get<ID>(ui::Tags::del))->Cancel();
				else if(cmd.Has(Ui::Tags::stop)) mgr->Get(cmd.Get<ID>(Ui::Tags::stop))->Pause();
				else if(cmd.Has(Ui::Tags::resume)) mgr->Get(cmd.Get<ID>(Ui::Tags::resume))->Resume();
				else if(cmd.Has(Ui::Tags::priority)) 
					mgr->Get(cmd.Get<ID>(Ui::Tags::id))->Priority(cmd.Get<unsigned int>(Ui::Tags::priority));
				System::GetDownloadMgr()->Update();
				break;
			}
			case Ui::Tags::dls:
			{
				Cat::PutDownloads(tpl);
				break;
			}
			case Ui::Tags::ups:
			{
				Cat::PutUploads(tpl);
				break;
			}
			case Ui::Tags::srs:
			{
				Cat::PutSearches(tpl);
				break;
			}
			case Ui::Tags::incoming:
			{
				Cat::PutIncoming(tpl);
				break;
			}
			case Ui::Tags::settings:
			{
				Cat::PutSettings(tpl);
				break;
			}
			case Ui::Tags::log:
			{
				Cat::PutLog(tpl);
				break;
			}
			case Ui::Tags::share:
			{
				uint page = 1; 
				if(cmd.Has(Ui::Tags::page)) page = cmd.Get<uint>(Ui::Tags::page);
				Cat::PutShare(tpl, page - 1);
				break;
			}
			case Ui::Tags::set:
			{
				OnSet(cmd, tpl);
				break;
			}

			case Ui::Tags::sr:
			{
				Search::Manager* mgr = System::GetSearchMgr();

				if(cmd.Has(Ui::Tags::dn)) CreateSearch(cmd);
				else if(cmd.Has(Ui::Tags::stop)) 
					mgr->Get(cmd.Get<ID>(Ui::Tags::stop))->SetRate(0); 
				else if(cmd.Has(Ui::Tags::resume))
					mgr->Get(cmd.Get<ID>(Ui::Tags::resume))->SetRate(Search::Manager::BaseRate()); 
				else if(cmd.Has(Ui::Tags::del))
				{
					SearcherPtr p = mgr->Get(cmd.Get<ID>(Ui::Tags::del));
					mgr->Detach(p);
				}
				break;
			}
			case Ui::Tags::srshow:
			{
				const ID id = cmd.Get<ID>(Ui::Tags::id);
				uint page = 1; 
				if(cmd.Has(Ui::Tags::page)) page = cmd.Get<uint>(Ui::Tags::page);
				Cat::PutSearchResults(tpl, id, page - 1);
				break;
			}
			case Ui::Tags::dlshow:
			{
				const ID id = cmd.Get<ID>(Ui::Tags::id);
				Cat::PutDownloadInfo(tpl, id);
				break;
			}
		}
	}

	static void CreateDownloadFromLink(const std::string& link)
	{
		if(starts_with(link, "ed2k://")) CntCreateDownloadFromEd2k(link);
		else if(starts_with(link, "magnet:?")) CntCreateDownloadFromMagnet(link);
		else throw Unhandled("Link type is not supported");
	}

	static void CreateDownloadFromSearch(ID sid, ID id)
	{
		SearcherPtr p = System::GetSearchMgr()->Get(sid);
		Search::Result r = p->GetResult(id); 

		DownloadRequest req;
		req.sha1 = r.hit.sha1;
		req.ttr = r.hit.ttr;
		req.md5 = r.hit.md5;
		req.ed2k = r.hit.ed2k;
		req.size = r.hit.size;
		req.name = r.hit.dn;

		DownloadPtr pDownload = System::GetDownloadMgr()->Create(req); 

		std::vector<Search::Result> results;
		p->DumpResults(std::back_inserter(results));
		for(std::size_t i = 0; i < results.size(); ++i)
		{
			const Search::Hit& tmp = results[i].hit;
			if(
				(!tmp.sha1.Empty() && tmp.sha1 == req.sha1) || 
				(!tmp.ttr.Empty() && tmp.ttr == req.ttr) || 
				(!tmp.md5.Empty() && tmp.md5 == req.md5) || 
				(!tmp.ed2k.Empty() && tmp.ed2k == req.ed2k)
			  )
			{
				pDownload->AddSource(results[i].host);
			}
		}
	}
	
	static void CreateSearch(const Ui::Command& cmd)
	{
		using boost::istarts_with;

		Search::Criteria criteria;

		const std::string dn = cmd.Get<std::string>(Ui::Tags::dn);
		if(istarts_with(dn, "urn:sha1:")) criteria.sha1.FromString(dn.substr(9));
		else if(istarts_with(dn, "urn:ttr:")) criteria.ttr.FromString(dn.substr(8));
		else if(istarts_with(dn, "urn:ed2k:")) criteria.ed2k.FromString(dn.substr(9));
		else if(istarts_with(dn, "urn:md5:")) criteria.md5.FromString(dn.substr(8));
		else criteria.dn = dn;

		if(cmd.Has(Ui::Tags::min))
			criteria.min = Ui::GetSizeValue(cmd.Get<std::string>(Ui::Tags::min));
		if(cmd.Has(Ui::Tags::max))
			criteria.max = Ui::GetSizeValue(cmd.Get<std::string>(Ui::Tags::max));
		if(cmd.Has(Ui::Tags::media))
		{
			if(cmd.Get<std::string>(Ui::Tags::media) != "any")
				criteria.media = StaticName::FromString(cmd.Get<std::string>(Ui::Tags::media));
		}
		if(cmd.Has(Ui::Tags::ext))
		{
			std::stringstream s(cmd.Get<std::string>(Ui::Tags::ext));
			std::string ext;
			while(s >> ext) criteria.extensions.insert(boost::to_lower_copy(ext));
		}
		System::GetSearchMgr()->Create(criteria, Search::Manager::USER);
	}

	static void OnSet(const Ui::Command& cmd, Ui::Template& t)
	{
		for(Ui::Command::Iterator i = cmd.Begin(); i != cmd.End(); ++i)
		{
			try{ System::GetSettings()->Set(i->first, i->second); }
			catch(std::exception& e)
			{
				System::LogBas() << "Option error: " << e.what() << std::endl;
				t.Put("set_error", std::string(e.what()));
			}
		}

		t.Put("set_result", "done");
	}
};
*/

#endif //DEFAULTREACTOR_HPP

