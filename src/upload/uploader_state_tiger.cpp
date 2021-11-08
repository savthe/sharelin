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

#include "uploader_state_tiger.hpp"
#include "uploader.hpp"
#include "verb.hpp"

namespace upload_details {

UploaderStateTiger::UploaderStateTiger(Uploader* u): uploader_(*u) {}

void UploaderStateTiger::start_transfer()
{
	const TigerTree& t = uploader_.file_->tiger_tree();
	std::vector<char> v(t.size_in_bytes());
	t.to_bytes(v.begin());
	buffer_.reset(v.begin(), v.end());
	verb3::upl << "Sending tiger tree of " << uploader_.file_->title() << " to " << uploader_.endpoint_ << std::endl;
	uploader_.sender_.send(buffer_, bind(&Uploader::on_data_sent, uploader_.ptr(), _1));
}

void UploaderStateTiger::on_sent()
{
	verb3::upl << "Tiger tree of " 
		<< uploader_.file_->title() << " has been transferred to " << uploader_.endpoint_ << std::endl;
}

void UploaderStateTiger::process_transfer(std::ostream& r)
{
	uploader_.request_.range.set(0, uploader_.file_->tiger_tree().size_in_bytes());
	r << "HTTP/1.1 200 OK";
	r << "\r\n";
	uploader_.ComposeCommonHeaders( r );
}

void UploaderStateTiger::print_request() const
{
	verb1::upl << "Host " << uploader_.endpoint_ << " requested tiger tree: " 
		<< uploader_.file_->title() << std::endl;
}

bool UploaderStateTiger::is_found() const
{
	return !uploader_.file_.expired() && !uploader_.file_->tiger_tree().empty();
}

file_offset_t UploaderStateTiger::size() const
{
	return uploader_.file_->tiger_tree().size_in_bytes();
}

} // namespace upload_details

