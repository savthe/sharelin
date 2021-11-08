/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009  Andrey Stroganov <savthe@gmail.com>

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

#include <set>
#include <algorithm>
#include <assert.h>
#include <stdexcept>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "manager.hpp"
#include "settings.hpp"
#include "filesystem.hpp"
#include "share.hpp"
#include "filemap.hpp"
#include "download.hpp"
#include "trafmanager.hpp"
#include "verb.hpp"
#include "tools.hpp"

using FileSystem::ScanDirectory;
using FileSystem::IsRegularFile;
using boost::starts_with;

namespace download_details {

inline int GetWeight(DownloadPtr p)
{
	using namespace dl_state;

	enum {LOW = 0, AVERAGE = 500, HIGH = 1000};

	int w = 0;
	switch(p->state())
	{
		case HASH:
		case MOVE:
		case ERROR:
			w = HIGH;
			break;
		case STOP:
		case TRANSFER:
			w = AVERAGE + p->priority();
			break;
		case PAUSE:
			w = LOW;
			break;
	}

	return w;
}

bool CompareDownloads(DownloadPtr p1, DownloadPtr p2)
{
	return GetWeight(p1) > GetWeight(p2);
	/*
	const Download::StatusType s1 = p1->Status();
	const Download::StatusType s2 = p2->Status();
	
	enum Weigth {HIGH, MIDDLE, LOW};
	int w1
	if(s1 == Download::HASH || s1 == Download::MOVE || s1 == Download::ERROR) return true;
	if(s1 == Download::STOP || s1 == Download::PAUSE) return false;
	if(s2 == Download::HASH || s2 == Download::MOVE || s2 == Download::ERROR) return false;
	

	if(s1 == s1 == Download::ERROR || s1 == Download::MOVE || s1 == Download::HASH)
		return true;

	if(s1 == Download::TRANSFER && (s2 == Download::TRANSFER || s2 == Download::STOP) && p1->Priority() >= p2->Priority())
		return true;

	if((s1 == Download::TRANSFER || s1 == Download::STOP) && (s2 == Download::TRANSFER || s2 == Download::STOP))
		return p1->Priority() > p2->Priority();

	return false;
	*/
}


Manager::Manager()
{
}

void Manager::Detach(DownloadPtr p)
{
	for(uint i = 0; i < downloads_.size(); ++i)
		if(downloads_[i] == p)
		{
			downloads_.erase(downloads_.begin() + i);
			break;
		}
	update_queue();
}

void Manager::onGiv(TcpSocketPtr pSock, ip::Endpoint endpoint, const std::string& message)
{
	const std::string::size_type sep = message.find(":");
	if(sep == std::string::npos) throw std::range_error("GIV separator not found");

	const std::string hex = message.substr(sep + 1, 32);
	if(hex.size() != 32) throw std::runtime_error("Bad hex code in GIV request");

	Guid guid;
	conv::Hex::decode(hex.begin(), hex.end(), guid.begin());

	for(Downloads::iterator i = downloads_.begin(); i != downloads_.end(); ++i)
	{
		if((*i)->has_source(guid))
		{
			(*i)->onGiv(pSock, endpoint); 
			return;
		}
	}
}

inf::DownloadManager Manager::info() const
{
	inf::DownloadManager i;
	BOOST_FOREACH(DownloadPtr p, downloads_)
		i.downloads.push_back(p->id());
	return i;
}

Id Manager::create(const DownloadRecipe& r)
{
	for(Downloads::iterator i = downloads_.begin(); i != downloads_.end(); ++i)
	{
		const FileInfo& f = (*i)->GetFileInfo();
		if(
			(!r.sha1.empty() && r.sha1 == f.sha1) ||
			(!r.ttr.empty() && r.ttr == f.ttr) ||
			(!r.md5.empty() && r.md5 == f.md5) ||
			(!r.ed2k.empty() && r.ed2k == f.ed2k) 
		  )
		{
			throw std::runtime_error("Alredy downloaded");
		}
	}

	if(r.sha1.empty() && r.ttr.empty() && r.md5.empty() && r.ed2k.empty())
		assert(!"Can't be so");

	FileInfo i;

	std::string name;
	if(!r.sha1.empty()) name = "sha1_" + r.sha1.toString();
	else if(!r.ttr.empty()) name = "ttr_" + r.ttr.toString();
	else if(!r.md5.empty()) name = "md5_" + r.md5.toString();
	else if(!r.ed2k.empty()) name = "ed2k_" + r.ed2k.toString();
	else throw std::runtime_error("No hash");

	i.name = r.name.empty() ? name : r.name;
	i.path = app::settings().general.incomplete + "/" + name;
	i.size = r.size;
	i.sha1 = r.sha1;
	i.ttr = r.ttr;
	i.md5 = r.md5;
	i.ed2k = r.ed2k;
	i.partial = idgen_.get();

	SharedFile f = app::share().add(i);
	f->create();

	DownloadPtr d(new Download(*this, f));
	downloads_.push_back(d);

	BOOST_FOREACH(const SourceHost& h, r.sources)
		d->add_source(h);

	update_queue();

	return i.partial;
}

DownloadPtr Manager::get(Id id) const 
{
	DownloadPtr p;
	BOOST_FOREACH(DownloadPtr p, downloads_)
		if(p->GetFileInfo().partial == id) return p;

	throw std::runtime_error("Bad Id");
}

void Manager::resume(Id id)
{
	get(id)->resume();
	update_queue();
}

inf::Download Manager::info_download(Id id) const
{
	return get(id)->info();
}

void Manager::cancel(Id id)
{
	DownloadPtr p(get(id));
	p->cancel();
	Detach(p);

	update_queue();
}

void Manager::pause(Id id)
{
	get(id)->pause();
	update_queue();
}

void Manager::set_priority(Id id, uint n)
{
	get(id)->priority(n);
	update_queue();
}

void Manager::Start()
{
	verb1::dl << "Loading downloads" << std::endl;

	sync_incomplete_folder();

	std::vector<SharedFile> v;
	app::share().dump(v);

	for(size_t i = 0; i < v.size(); ++i)
	{
		SharedFile f = v[i];
		assert(f);
		if(!starts_with(f->path(), app::settings().general.incomplete)) continue;

		verb1::dl << "Loading: " << f->path() << std::endl;
		
		f->partial(idgen_.get());
		
		DownloadPtr d(new Download(*this, f));
		d->load();
		downloads_.push_back(d);
	}	

	update_queue();

	verb1::dl << downloads_.size() << " download(s) created." << std::endl;
}

void Manager::update_queue()
{
	using namespace dl_state;

	std::stable_sort(downloads_.begin(), downloads_.end(), CompareDownloads);
	uint active = app::settings().net.maxFiles;

	std::size_t i = 0;
	for(i = 0; i < downloads_.size() && active; ++i)
	{
		DownloadPtr p = downloads_[i];
		if(p->state() == STOP)
			p->resume();
		if(p->state() == TRANSFER)
			--active;
	}

	for(; i < downloads_.size(); ++i)
	{
		DownloadPtr p = downloads_[i];
		if(p->state() == TRANSFER)
			p->stop();
	}
}

void Manager::Shutdown()
{
	BOOST_FOREACH(DownloadPtr p, downloads_) p->shutdown();
	downloads_.clear();
}

} // namespace download_details

