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

#include <boost/bind.hpp>

#include "updateswatch.hpp"
#include "appdefs.hpp"
#include "app.hpp"
#include "verb.hpp"
#include "history.hpp"

UpdatesWatch::UpdatesWatch(ba::io_service& io):
	pAnt_(new HttpAnt(io, 16 * 1024)),
	periodFetch_(boost::posix_time::hours(3))
{
}

void UpdatesWatch::OnFinished(const HttpAnt::Error& err)
{
	using namespace inf::history;

	if(err)
	{
		verb1::app << "Couldn't fetch latest version info ==> " << err.Message() << std::endl;
		return;
	}

	verb1::app << "Successfuly fetched latest version info" << std::endl;
	std::stringstream s;
	std::ostream_iterator<char> i(s);
	pAnt_->CopyContent(i);

	RecentVersion rv;
	std::string line;
	if(s >> rv.birthday >> rv.tag >> rv.link)
	{
		verb1::app << "Latest version tag: " << rv.tag << std::endl;

		while(std::getline(s, line))
		{
			if(line.empty()) continue;
			rv.comments += line + '\n';
		}

		app::history().recent_version(rv);
	}
	else verb1::app << "Couldn't parse updates host output" << std::endl;
}

void UpdatesWatch::OnTimer()
{
	if(periodFetch_.set())
	{
		verb1::app << "Fetching latest version info" << std::endl;
		pAnt_->Run("http://sharelin.sourceforge.net/version.dat", bind(&UpdatesWatch::OnFinished, this, _1));
	}
}

void UpdatesWatch::Shutdown()
{
	if(!pAnt_->Closed()) pAnt_->Cancel();
}

