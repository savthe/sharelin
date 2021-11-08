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

#ifndef HTTPANT_HPP
#define HTTPANT_HPP

#include <vector>
#include <string>
#include <algorithm>

#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "io.hpp"
#include "url.hpp"

class HttpAnt: public boost::enable_shared_from_this<HttpAnt>
{
public:
	enum ErrorType { E_NONE, E_TIMEOUT, E_RESOLVE, E_TRANSFER, E_LOGIC, E_SYSTEM, E_CONNECT };
	class Error
	{
	public:
		Error(ErrorType e): err_(e) {}
		Error(ErrorType e, const std::string& msg): err_(e), msg_(msg) {}
		ErrorType Type() const { return err_; }
		std::string Message() const { return msg_; }
		operator bool() const { return err_ != E_NONE; }


	private:
		ErrorType err_;
		std::string msg_;
	};
	typedef boost::function<void (const Error&)> CompletionFunctor;


	HttpAnt(ba::io_service&, uint);
	void Run(const std::string&, CompletionFunctor);
	void Cancel();
	unsigned int ContentLength() const { return content_.size(); }
	bool Closed() const { return closed_; }

	template <typename T>
	void CopyContent(T begin)
	{
		std::copy(content_.begin(), content_.end(), begin);
	}

private:
	void Run(const bs::error_code& err);
	void ScheduleRun();
	void OnResolved(const bs::error_code& err, ba::ip::tcp::resolver::iterator iep);
	void OnConnected(const bs::error_code& err, ba::ip::tcp::resolver::iterator iep);
	void OnWrite(const bs::error_code& err);
	void OnHeadersReceived(const bs::error_code& err);
	void OnContent(const bs::error_code& err);
	void Timeout(const bs::error_code& err);
	void Finish(const Error&);
	void HandleReply();
	

	ba::ip::tcp::socket socket_;
	ba::ip::tcp::resolver resolver_;
	ba::deadline_timer timer_;
	ba::streambuf request_;
	ba::streambuf reply_;
	std::vector<char> content_;
	bool closed_;
	CompletionFunctor completer_;
	unsigned int contentMaxLength_;
	unsigned int waitTime_;
	unsigned int contentLength_;
	
};

#endif //HTTPANT_HPP

