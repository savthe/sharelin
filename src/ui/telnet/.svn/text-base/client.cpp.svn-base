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

#include <algorithm>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "client.hpp"
#include "server.hpp"
#include "../common/server.hpp"
#include "settings.hpp"
#include "../common/query.hpp"
#include "../common/template.hpp"
#include "ipsegment.hpp"
#include "settings.hpp"
#include "appdefs.hpp"
#include "tools.hpp"
#include "verb.hpp"
#include "io.hpp"

using boost::iequals; 
using boost::trim;
namespace ui {
namespace telnet {

Client::Client(Server* pServer):
	socket_(app::io()), 
	message_(64 * 1024),
	closed_(false),
	pServer_(pServer)
{
}

std::string Client::GetPromt() const
{
	Template t;
	std::string promt = std::string(app::name) + ": ";
	t.Load(pServer_->StyleRoot() + "/promt.tpl");
	t.Format(promt);
	FormatAscii(promt);

	return promt;
}

ip::Endpoint Client::GetEndpoint()
{
	ip::Endpoint e(socket_.remote_endpoint().address().to_v4().to_ulong(), socket_.remote_endpoint().port());
	e.address.reverse();
	return e;
}

void Client::Start()
{
	const ip::Address addr = GetEndpoint().address;

	bool found = false;
	BOOST_FOREACH(IpSegment s, app::settings().general.uisegments)
		found |= s.contains(addr);

	if(!found)
	{
		std::ostream r(&message_);
		r << "IP not allowed" << "\r\n";
		ba::async_write(socket_, message_, 
				boost::bind(&Client::OnFinal, shared_from_this(), ba::placeholders::error));
	}
	else 
	{
		reply_ += GetPromt();
		ba::async_write(socket_, ba::buffer(reply_), boost::bind(&Client::OnReplySent, shared_from_this(), ba::placeholders::error));
	}
	//	ba::async_read_until(socket_, message_, "\r\n", 
	//		boost::bind(&TelnetClient::OnData, shared_from_this(), ba::placeholders::error));
}

void Client::OnData(const bs::error_code& err)
{
	if(closed_) return;

	if(!err)
	{
		std::istream r(&message_);
		std::string line;
		std::getline(r, line);
		trim(line);
		verb1::ui << "Got telnet query from " << GetEndpoint() << " => " << line << std::endl;
		if(iequals(line, "exit") || iequals(line, "\x04"))
		{
			DetachMe();
			return;
		}
		else ProcessQuery(line);

	}
	else if(err != ba::error::operation_aborted)
	{
		verb1::ui << "Telnet client " << GetEndpoint() << " => " << err.message() << std::endl;
		DetachMe();
		return;
	}

}

void Client::OnReplySent(const bs::error_code& err)
{
	if(closed_) return;
	if(!err) ba::async_read_until(socket_, message_, "\r\n", boost::bind(&Client::OnData, shared_from_this(), ba::placeholders::error));
	else if(err != ba::error::operation_aborted)
	{
		verb1::ui << "Telnet client " << GetEndpoint() << " => " << err.message() << std::endl;
		DetachMe();
		return;
	}
}

void Client::stop()
{
	if(!closed_)
	{
		if(socket_.is_open()) socket_.close();
		closed_ = true;
	}
}

void Client::DetachMe()
{
	boost::shared_ptr<Client> me(shared_from_this());
	stop();
	pServer_->Detach(me);
}

void Client::ProcessQuery(const std::string& sq)
{
	//cmd.FromString(scmd_, ' ');

	Query query;
	std::string result;

	try 
	{ 
		query.FromString(sq, ' ');
		pServer_->ProcessQuery(query, result);
		//const std::string root = app::GetTelnet()->GetStyleRoot();
		//if(root.empty()) throw std::runtime_error("Couldn't find telnet style folder. Please set valid style name in sharelin.conf");
		//cmd.FromString(scmd, ' '); 
	//	UiControl cnt(root, app::GetWebui()->GetPrefs());
	//	cnt.Run(cmd, result);
	}
	catch(std::exception& e)
	{
		verb1::ui << "Telnet query error: " << sq << " => " << e.what() << std::endl;
		result = std::string("Query error: ") + std::string(e.what());
	}

	//UiControl cnt(cmd, result);
	FormatAscii(result);
	result += "\r\n" + GetPromt();
	reply_.clear();
	reply_.reserve(result.size());
	std::copy(result.begin(), result.end(), std::back_inserter(reply_));

	ba::async_write(socket_, ba::buffer(reply_), boost::bind(&Client::OnReplySent, shared_from_this(), ba::placeholders::error));
}

void Client::OnFinal(const bs::error_code& err)
{
	if(closed_) return;

	if(err != ba::error::operation_aborted)
		DetachMe();
}

} // namespace telnet
} // namespace ui

