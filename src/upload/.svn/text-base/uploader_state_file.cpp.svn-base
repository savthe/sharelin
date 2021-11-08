/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2013  Andrey Stroganov <savthe@gmail.com>

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

#include "uploader_state_file.hpp"
#include "uploader.hpp"
#include "verb.hpp"
#include "type2stream.hpp"

namespace upload_details {

UploaderStateFile::UploaderStateFile(Uploader* u): uploader_(*u) {}

void UploaderStateFile::on_sent()
{
	verb3::upl << "Range " << buffer_.range() << " of " 
		<< uploader_.file_->title() << " has been transferred to " << uploader_.endpoint_ << std::endl;
}

void UploaderStateFile::start_transfer()
{
	uploader_.file_.open();
	buffer_.reset(uploader_.file_, uploader_.request_.range);

	verb3::upl << "Sending range " << buffer_.range() 
		<< " of " << uploader_.file_->title() << " to " << uploader_.endpoint_ << std::endl;

	uploader_.sender_.send(buffer_, bind(&Uploader::on_data_sent, uploader_.ptr(), _1));
}

void UploaderStateFile::process_transfer(std::ostream& r)
{
	const file_offset_t file_size = uploader_.file_->size();
	ByteRange& range = uploader_.request_.range;

	if( range.empty() ) 
		range.set_bound(0, file_size - 1);

	if( range.last() >= file_size ) 
		throw Uploader::Unhandled(416, "Range is too large" );

	const bool partial = range.length() < file_size;
	if( !partial ) r << "HTTP/1.1 200 OK";
	else r << "HTTP/1.1 206 Partial content";
	r << "\r\n";
	uploader_.ComposeCommonHeaders( r );
	if( partial ) r << "Content-Range: bytes=" << range << "/" << file_size << "\r\n"; 

	const TigerTree& t = uploader_.file_->tiger_tree();
	if( !t.empty() )
		r << "X-Thex-URI: /gnutella/thex/v3?urn:tree:tiger/:"
			<< uploader_.file_->ttr().toString() << "&depth=" << t.depth() << ";" << "\r\n";

}

void UploaderStateFile::print_request() const
{
	verb1::upl << "Host " << uploader_.endpoint_ << " requested file: " 
		<< uploader_.file_->title() << std::endl;
}

bool UploaderStateFile::is_found() const
{
	return !uploader_.file_.expired();
}

file_offset_t UploaderStateFile::size() const
{
	return uploader_.file_->size();
}

} // namespace upload_details

