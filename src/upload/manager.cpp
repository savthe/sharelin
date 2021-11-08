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

#include <boost/foreach.hpp>

#include "manager.hpp"
#include "settings.hpp"
#include "history.hpp"
#include "g2queueinfo.hpp"
#include "verb.hpp"

namespace upload_details {

Manager::Manager()
{
}

void Manager::Shutdown()
{
	BOOST_FOREACH(Uploader::Ptr p, uploaders_)
		p->Close();
	uploaders_.clear();
}

void Manager::debug()
{
}

void Manager::OnTimer( std::time_t now )
{
	std::vector< Uploader::Ptr > v = uploaders_;
	BOOST_FOREACH( Uploader::Ptr p, v )
		p->OnTimer( now );
}

void Manager::Detach( Uploader::Ptr p )
{
	for( uint i = 0; i < uploaders_.size(); ++i )
		if( uploaders_[i] == p )
		{
			verb3::upl << "Detaching uploader " << p->Address() << std::endl;
			uploaders_.erase( uploaders_.begin() + i );
			break;
		}
	UpdateQueue();
}

inf::UploadManager Manager::info() const
{
	inf::UploadManager m;

	for(size_t i = 0; i < uploaders_.size(); ++i)
		m.uploaders.push_back(uploaders_[i]->info());

	return m;
}

void Manager::onGet(TcpSocketPtr pSock, ip::Endpoint endpoint, const std::string& message)
{
	verb1::upl << "Got upload request from " << endpoint << std::endl;

	uint count = 0;
	BOOST_FOREACH( Uploader::Ptr p, uploaders_ )
		if(endpoint == p->Address()) ++count;

	if( count >= app::settings().net.uploadsPerHost ) 
	{
		verb2::upl << "Refusing uploader connection " << endpoint << " (per host limit)" << std::endl;
		return;
	}

	if( uploaders_.size() > app::settings().net.queueSize + app::settings().net.maxUploads)
	{
		verb2::upl << "Refusing uploader connection " << endpoint << " (uploads limit)" << std::endl;
		return;
	}

	Uploader::Ptr p(new Uploader(*this, pSock, endpoint));
	uploaders_.push_back( p );
	UpdateQueue();

	try
	{
		p->Start(message);
	}
	catch(std::exception& e)
	{
		verb1::upl << "Couldn't start upload to " << endpoint << " : " << e.what() << std::endl;
		Detach(p);
	}
}

void Manager::UpdateQueue()
{
	const uint max = app::settings().net.maxUploads;
	const uint queueSize = uploaders_.size() <= max ? 0 : uploaders_.size() - max;

	g2::QueueInfo q;
	q.id = "general";
	q.limit = app::settings().net.maxUploads;
	q.pollMin = app::settings().net.pollMin;
	q.pollMax = app::settings().net.pollMax;
	q.length = queueSize;

	for(unsigned i = 0; i < uploaders_.size(); ++i)
	{
		Uploader::Ptr p = uploaders_[i];
		q.position = i < max ? 0 : i + 1 - max;
		p->Enqueue( q );
		//if(i < max) p->SetQueue(0, queueSize);
		//else p->SetQueue(i + 1 - max, queueSize);	
	}

}

} // namespace upload_details
