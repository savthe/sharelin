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

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include "uploader.hpp"
#include "manager.hpp"
#include "share.hpp"
#include "appdefs.hpp"
#include "settings.hpp"
#include "trafmanager.hpp"
#include "security.hpp"
#include "uploadfilebuffer.hpp"
#include "type2stream.hpp"
#include "history.hpp"
#include "verb.hpp"
#include "debug.hpp"
#include "util.hpp"
#include "app.hpp"
#include "traf.hpp"
#include "util.hpp"
#include "uploader_state_file.hpp"
#include "uploader_state_tiger.hpp"

using boost::istarts_with;
using boost::starts_with;
using boost::trim_copy;
using boost::iequals;
using util::since;

namespace upload_details {

const uint BULK_SIZE = 4096;
const uint MIN_AVERAGE = 10;
const std::time_t TRANSFER_TIMEOUT = BULK_SIZE / MIN_AVERAGE;

Uploader::Uploader(Manager& m, TcpSocketPtr pSock, ip::Endpoint endpoint):
	manager_(m),
	pSock_(pSock),
	endpoint_(endpoint),
	traf_(app::traf_out()),
	sender_(*pSock_, traf_),
	requestBuf_(16 * 1024),
	status_(IDLE)
{
	timestamp_ = time(0);
	//pGatekeeper->CopyRequest(out);
}

Uploader::~Uploader()
{
	assert(status_ == CLOSED);
}

inf::Uploader Uploader::info() const
{
	inf::Uploader i;

	i.endpoint = endpoint_;
	i.nick = request_.nick;
	i.title = file_->title();
	i.sent = traf_.total();
	i.rate = traf_.rate();
	i.queued = qinfo_.Enqueued();

	return i;
}

void Uploader::Start(const std::string& message)
{
	verb3::upl << "Starting uploader " << endpoint_ << std::endl;
	timestamp_ = util::now();
	std::ostreambuf_iterator<char> out(&requestBuf_);
	std::copy(message.begin(), message.end(), out);
	HandleRequest();
	traf_.consume(message.size());
}

void Uploader::OnRequest( const bs::error_code& err )
{
	if(status_ == CLOSED) return;

	if( !err )
	{
		traf_.consume(requestBuf_.size());
		try
		{
			HandleRequest();
			timestamp_ = util::now();
		}
		catch(std::exception& e)
		{
			verb1::upl << "Couldn't handle upload request in " << endpoint_ << " : " << e.what() << std::endl;
			DetachMe();
			return;
		}
	}
	else if( err != ba::error::operation_aborted )
	{
		verb3::upl << "Uploader::OnRequest " << endpoint_ << " error => " << err.message() << std::endl;
		DetachMe();
	}
}

Uploader::Ptr Uploader::ptr()
{
	return shared_from_this();
}

void Uploader::setup_file()
{
	const size_t first_separator = request_.urn.find('&');
	file_ = app::share().find_urn( request_.urn.substr(0, first_separator)); 
}

void Uploader::process_request(std::ostream& ostr)
{
	verb3::upl << "Processing uploading request from " << endpoint_ << std::endl;

	request_.range.reset();
	const std::string last_file = !file_.expired() ? file_->path() : std::string();
	const g2::HttpRequest last_request = request_;

	std::istream s( &requestBuf_ );

	std::string line;

	s >> line; //method

	if( line == "GET" ) request_.method = g2::HttpRequest::GET;
	else if( line == "HEAD" ) request_.method = g2::HttpRequest::HEAD;
	else throw Unhandled(400, "Unknown method" );

	s >> line; //object
	verb1::upl << "Got request: " << line << std::endl;
	if( starts_with( line, "/uri-res/N2R?" ) ) 
		state_.reset(new UploaderStateFile(this));
	else if( starts_with( line, "/gnutella/thex/v3?" ) || starts_with( line, "/gnutella/tigertree/v3?" )) 
	{
		verb1::upl << "Switching to tiger tree upload state: " << line << std::endl;
		state_.reset(new UploaderStateTiger(this));
	}
	else 
	{
		verb1::upl << "Unsupported upload request: " << line << std::endl;
		throw Unhandled(400, "Requested uri type is not supported");
	}

	request_.urn = line.substr(line.find('?') + 1);
	setup_file();
	if( !file_ || !state_->is_found() )
		throw Unhandled(404, "Not found");

	state_->print_request();

	if(last_file != file_->path())
	{
		if(!last_file.empty() && traf_.total())
			app::history().Add(inf::history::Upload(last_request.nick, last_file, 
					last_request.client, traf_.total(), endpoint_));
		file_->increase_requests();
	}


	while( std::getline( s, line ) && line != "\r" )
	{
		std::string value = boost::trim_copy( line.substr( line.find( ':' ) + 1 ) );
	
		if( istarts_with( line, "Connection:" ) ) 
			request_.keepAlive = iequals( value, "keep-alive" );
		else if( istarts_with( line, "X-Nick:" ) ) 
			request_.nick = value;
		else if(istarts_with(line, "User-Agent:") && request_.client != value)
		{
			request_.client = value;
			if(app::security().AgentRestricted(request_.client))
				throw Unhandled(403, "Client software is restricted");
		}
		else if( istarts_with( line, "Range:" ) )
		{
			file_offset_t first = 0;
			file_offset_t last = 0;

			int result = sscanf( value.c_str(), "bytes=%llu-%llu", &first, &last );
			if( result == 0 ) throw Unhandled(416, "Couldn't parse range");
			if( result == 1 ) request_.range.set_bound( first, state_->size() - 1);
			if( result == 2 ) request_.range.set_bound( first, last );
		}
	}

	if( qinfo_.Enqueued() ) 
		process_queued(ostr);
	else
	{
		verb3::upl << "Starting data transfer to " << endpoint_ << std::endl;
		status_ = TRANSFER;
		state_->process_transfer(ostr);

		ostr << "Content-Length: " << (request_.method == g2::HttpRequest::GET ? request_.range.length() : 0) << "\r\n"; 
	}
	/*
	if( request_.range.empty() ) 
		request_.range.set_bound(0, file_->size() - 1);

	if( file_->partial() && file_->map().status(request_.range) != Integrity::VALID )
			throw Unhandled(416, "Requested Range Not Satisfiable");
			*/

	//if( request_.range.empty() ) throw std::range_error( "Range is empty" );
}

void Uploader::process_queued(std::ostream& r)
{
	verb2::upl << "Queuing uploader " << endpoint_ << ": " 
		<< qinfo_.position << " of " << qinfo_.length << std::endl;

	status_ = QUEUED;

	r << "HTTP/1.1 503 Busy queued\r\n";
	ComposeCommonHeaders(r);
	r << qinfo_.Header() << "\r\n";
	r << "Content-Length: 0\r\n";
}

void Uploader::HandleRequest()
{
	//range_.Reset();
//	offset_ = 0;
	request_.keepAlive = true;

	std::ostream r( &responseBuf_ );

	try
	{
		try
		{ 
			if(status_ == QUEUED && util::now() - timestamp_ < std::time_t(qinfo_.pollMin))
				throw Unhandled(503, "Polling too often");

			process_request(r);
		}
		catch(Unhandled& e)
		{
			r << "HTTP/1.1 " << e.Status() << " " << e.what() << "\r\n";
			throw;
		}
		catch( std::exception& e )
		{
			r << "HTTP/1.1 500 Internal server error\r\n";
			throw;
		}
	}
	catch(std::exception& e)
	{
		verb1::upl << "Request error in uploader " << endpoint_ << " " << e.what() << std::endl;
		status_ = CLOSING;
	}

	r << "\r\n";

	ba::async_write( *pSock_, responseBuf_, 
			boost::bind( &Uploader::OnResponseSent, shared_from_this(), ba::placeholders::error ) );
}

void Uploader::OnResponseSent( const bs::error_code& err )
{
	if(status_ == CLOSED) return;

	if(!err && status_ != CLOSING && !file_.expired())
	{
		if(status_ == TRANSFER && request_.method == g2::HttpRequest::GET)
			state_->start_transfer();
		else 
		{
			if(status_ == TRANSFER)
			{
				assert(request_.method == g2::HttpRequest::HEAD);
				status_ = IDLE;
				verb3::upl << "Uploader " << endpoint_ << " received HEAD reply containing range " 
					<< request_.range << " of " << file_->title() << " to " << endpoint_ << std::endl;
			}
			else assert(status_ == QUEUED);

			ba::async_read_until( *pSock_, requestBuf_, "\r\n\r\n", 
					boost::bind( &Uploader::OnRequest, shared_from_this(), ba::placeholders::error ) );
		}
	}
	else if(err != ba::error::operation_aborted)
	{
		if( status_ != CLOSING )
		{
			if(file_.expired()) 
				verb3::upl << "Terminating upload to " << endpoint_ << " => File is no longer available" << std::endl;
			else verb3::upl << "Uploader::OnResponseSent " << endpoint_ << " error => " << err.message() << std::endl;
		}
		DetachMe();
	}
}

/*
void Uploader::Transfer()
{
	timestamp_ = util::now();

	assert(pBuffer_);
	const file_offset_t want = std::min(pBuffer_->BytesLeft(), BULK_SIZE); 
	const uint reserved = want;//app::GetTraffOut()->Use(endpoint_, want);

	if(reserved)
	{
		bwlimit_ = false;
		outData_.resize(reserved);
		try{ pBuffer_->Read(outData_.begin(), reserved); }
		catch(std::exception& err)
		{
			verb1::upl << "File reading error in uploader: " << err.what() << std::endl;
			throw;
		}

		traf_.consume(outData_.size());
		//totalSent_ += outData_.size();

		ba::async_write( *pSock_, ba::buffer( outData_ ), boost::bind( &Uploader::OnTransfer, shared_from_this(), ba::placeholders::error ) );
	}
	else bwlimit_ = true;
}
*/

void Uploader::on_data_sent(const bs::error_code& err)
{
	if(status_ == CLOSED) return;

	if(!err && !file_.expired())
	{
		timestamp_ = util::now();
		status_ = IDLE;

		state_->on_sent();
		state_.reset();

		if(!request_.keepAlive)
		{
			verb1::upl << "Disconnecting uploader " << endpoint_ << " => connection: close" << std::endl;
			DetachMe();
			return;
		}

		ba::async_read_until( *pSock_, requestBuf_, "\r\n\r\n", 
				boost::bind( &Uploader::OnRequest, shared_from_this(), ba::placeholders::error ) );
	}
	else if( err != ba::error::operation_aborted )
	{
		if(file_.expired()) 
			verb3::upl << "Terminating upload to " << endpoint_ << " => File is no longer available" << std::endl;
		else verb1::upl << "Uploader::OnTransfer " << endpoint_ << " error => " << err.message() << std::endl;
		DetachMe();
	}
}

void Uploader::ComposeCommonHeaders(std::ostream& r)
{
	r << "Host: " << endpoint_ << "\r\n";
	if( request_.keepAlive ) r << "Connection: Keep-Alive" << "\r\n";
	r << "X-Features: g2/1.0" << "\r\n";
	r << "User-Agent: " << app::fullname << "\r\n";
	r << "Listen-IP: " << app::endpoint() << "\r\n";
	r << "X-Nick: " << app::settings().general.userName << "\r\n";
}

void Uploader::OnTimer( std::time_t now )
{
	try
	{
		const std::time_t timeout = (status_ == QUEUED ? app::settings().net.pollMax : TRANSFER_TIMEOUT);

		if(since(traf_.last_activity()) > timeout )
		{
			verb1::upl 
				<< "Uploader " << endpoint_ << " stalled (" << timeout << "). "  
				<< "Status: " << (status_ == QUEUED ? "queued" : "transfer") << ". ";
			if(status_ == TRANSFER && request_.method == g2::HttpRequest::GET)
				verb1::upl << "Range: " << request_.range << ". ";

			verb1::upl << std::endl;

			Close();
			DetachMe();
			return;
		}
	}
	catch(std::exception& err)
	{
		verb1::upl << "Exception in uploader " << endpoint_ << " => " << err.what() << std::endl;
		Close();
		DetachMe();
	}
}

void Uploader::DetachMe()
{
	Close();
	Ptr me = shared_from_this();
	manager_.Detach( me );
}

void Uploader::Close()
{
	if(!file_.expired() && traf_.total())
		app::history().Add(inf::history::Upload(request_.nick, 
					file_->path(), request_.client, traf_.total(), endpoint_));

	if(status_ != CLOSED)
	{
		bs::error_code e;
		if(pSock_->is_open()) pSock_->close(e);
		if(e) verb1::upl << "Error during closing socket in uploader" << std::endl;
		//if(!file_.IsClosed()) file_.Close();
		status_ = CLOSED;
	}
}

} // namespace upload_details
