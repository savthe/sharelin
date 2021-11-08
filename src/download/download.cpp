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

#include <vector>
#include <stdexcept>
#include <sstream>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "download.hpp"
#include "filesystem.hpp"
#include "hashes.hpp"
#include "share.hpp"
#include "settings.hpp"
#include "app.hpp"
#include "conv/conv.hpp"
#include "asynchash.hpp"
#include "manager.hpp"
#include "history.hpp"
#include "verb.hpp"
#include "util.hpp"
#include "downloader.hpp"
#include "transfer.hpp"
#include "task_server.hpp"

namespace download_details {

using FileSystem::FileName;
using FileSystem::FileSize;
using FileSystem::MakeAvailableName;
using FileSystem::Unlink;

using boost::starts_with;
using boost::iequals;

using util::now;
using util::since;
using namespace dl_state;

Download::Download(Manager& m, SharedFile f):
	manager_(m),
	file_(f), 
	transfer_(new Transfer(f)),
	state_(STOP),
	async_hash_(app::task_server()),
	async_copy_(app::task_server())
{
}

Download::~Download() {}

void Download::pause()
{
	stop();
	state_ = PAUSE;
	save();
}

inf::Download Download::info() const 
{
	inf::Download i;

	i.f = file_->info();
	i.rate = transfer_->rate();
	i.priority = transfer_->priority();
	i.state = state_;
	i.sources_count = transfer_->sources().size();

	std::vector<Downloader::Ptr> tmp(transfer_->dump_downloaders());
	BOOST_FOREACH(Downloader::Ptr p, tmp)
		i.downloaders.push_back(p->info());

	return i;
}

void Download::cancel()
{
	stop();
	Unlink(InfPath());
	const std::string path(file_->path());
	file_->shutdown();
	app::share().remove(path);
	Unlink(path);
}

void Download::priority(int8 n)
{
	transfer_->priority(n);
}

int8 Download::priority() const 
{
	return transfer_->priority();
}

bool Download::has_source(Guid guid) const
{
	const Sources& s = transfer_->sources();
	Sources::const_iterator i = s.begin();

	while(i != s.end() && i->second.guid != guid) ++i;

	return i != s.end();
}

void Download::onGiv(TcpSocketPtr pSock, ip::Endpoint endpoint)
{
	if(state_ != TRANSFER) return;
	transfer_->accept(pSock, endpoint);
}

void Download::DetachMe()
{
	if(file_) verb1::dl << "Detaching download of file " << file_->path() << std::endl;
	else verb1::dl << "Detaching download" << std::endl;
	async_hash_.cancel();
	async_copy_.cancel();
	transfer_->stop();
	manager_.Detach(shared_from_this());
}

void Download::finalize()
{
	std::string name = file_->title();
	if(name.empty()) name = DefaultURN();
	if(file_->map().status() == Integrity::INVALID)
	{
		verb1::dl << "File " << name << " has been downloaded, but may be corrupted. Moving." << std::endl;
		name = "CORRUPTED_" + name;
	}
	else
	{
		assert(file_->map().status() == Integrity::VALID);
		verb1::dl << "File " << name << " has been successfully downloaded. Moving." << std::endl;
	}

	state_ = MOVE;
	file_->sync();
	std::string newName = MakeAvailableName(app::settings().general.complete + "/" + name);
	verb1::dl << "Copying file " << file_->path() << " to " << newName << std::endl;
	async_copy_.copy(file_->path(), newName, bind(&Download::on_copied, this, _1));
//	work_.Run(async_copy::Task(file_.path(), newName, boost::bind(&Download::Copied, this, _1)));
}

void Download::on_copied(const boost::system::error_code& err)
{
	const std::string to = async_copy_.to();
	const std::string from = async_copy_.from();

	if(!err)
	{
		stop();
		try 
		{ 

			Unlink(InfPath());

			file_->finalize_partial(to);
			
			inf::history::Download hist(to, file_->size());
			app::history().Add(hist);

			verb1::dl << "File " << to << " is ready to use" << std::endl;

			Unlink(from);
		}
		catch ( std::exception& e )
		{
			verb1::dl << "File moving error. Transfer problem: " << e.what() << std::endl;
			DetachMe();
		}
		DetachMe();
	}
	else
	{
		verb1::dl << "File copy error. From: " << from << " to: " << to << " error: " << err.message() << std::endl;
		state_ = ERROR;
	}
}

void Download::on_hashed(const boost::system::error_code& err)
{
	ByteRange r = async_hash_.range();

	if(err)
	{
		file_->set_invalid(r);
		return;
	}

	assert(r.length() == file_->size());
	assert(r.offset() == 0);

	const hash::SHA1 sha1 = async_hash_.sha1();
	const hash::TTR ttr = async_hash_.ttr();
	const hash::MD5 md5 = async_hash_.md5();
	const hash::ED2K ed2k = async_hash_.ed2k();

	bool match = false;
	match |= !sha1.empty() && sha1 == file_->sha1(); 
	match |= !ttr.empty() && ttr == file_->ttr(); 
	match |= !md5.empty() && md5 == file_->md5(); 
	match |= !ed2k.empty() && ed2k == file_->ed2k(); 

	if(match)
	{
	//	verb.bas << "Setting range VALID: " << r.First() << "-" << r.Last() << verb.flush;
		file_->set_valid(r);
	}
	else
	{
		verb2::dl << "Invalidating " << file_->path() << " Range: " << r.first() << "-" << r.last() << std::endl;
	//	verb.bas << result.ttr.ToString() << verb.flush;
	//	verb.bas << fileInfo_.tigerTree.Get(r).ToString() << verb.flush;
		file_->set_invalid(r);
	}

	finalize();
}

void Download::on_transferred()
{
	const Integrity s = file_->map().status();

	if(s == Integrity::FULL)
	{
		state_ = HASH;
		file_->sync();
	//	work_.Run(async_hash::Task(file_.path(), hash::Mask::ANY, ByteRange(0, file_.size()), boost::bind(&Download::Hashed, this, _1)));
		async_hash_.run(file_->path(), hash::Mask::ANY, ByteRange(0, file_->size()), 0, bind(&Download::on_hashed, this, _1));
	}
	else finalize();
}

std::string Download::DefaultURN() const
{
	if(!file_->sha1().empty() )
		return "sha1:" + file_->sha1().toString();
	if(!file_->ed2k().empty())
		return "ed2k:" + file_->ed2k().toString();
	if(!file_->ttr().empty())
		return "ttr:" + file_->ttr().toString();
	if(!file_->md5().empty())
		return "md5:" + file_->md5().toString();
	return std::string();
}

std::string Download::InfPath()
{
	return app::settings().general.incomplete + "/inf/" + FileName(file_->path()) + ".inf";
}

void Download::load()
{
	std::ifstream f(InfPath().c_str());
	std::string s;
	while(f >> s)
	{
		if(s == "priority")
		{
			int8 n;	
			f >> n;
			transfer_->priority(n);
		}
		else if(s == "status")
		{
			std::string tmp;
			f >> tmp;
			if(tmp == "paused") state_ = PAUSE;
			else state_ = STOP;
		}
		else if(s == "source")
		{
			std::string addr;
			f >> addr;
			SourceHost src;
			if(src.endpoint.fromString(addr))
				add_source(src);
		}
	}
}

void Download::save()
{
	std::ofstream f(InfPath().c_str());

	if(!f.good())
	{
		verb1::dl << "Can't open file for writing: " << InfPath() << std::endl;
		return;
	}

	f << "priority " << transfer_->priority() << std::endl;
	
	f << "status ";
	switch (state_)
	{
		case PAUSE: f << "paused"; break;
		default: f << "transfer";
	}
	f << std::endl;

	for(Sources::const_iterator i = transfer_->sources().begin(); i != transfer_->sources().end(); ++i)
	{
		f << "source " << i->second.endpoint << std::endl;
	}

	f.close();
}

void Download::resume()
{
	if( !(state_ == STOP || state_ == PAUSE) ) 
		throw BadStatus("Download is not stopped or paused");

	//assert(state_ == STOP || state_ == PAUSE);
	try { file_.open(); }
	catch (std::exception& e)
	{
		verb1::dl << "Download resume error: " << e.what() << std::endl;
		state_ = ERROR;
		return;
	}

	state_ = TRANSFER;
	transfer_->start(boost::bind(&Download::on_transferred, this));
}

void Download::stop()
{
	file_.close();
	transfer_->stop();
	async_copy_.cancel();
	async_hash_.cancel();
	state_ = STOP;
}

void Download::shutdown()
{
	stop();
	save();
}

void Download::add_source(const SourceHost& source)
{
	transfer_->add_source(source);
}

} // namespace download_details
	
