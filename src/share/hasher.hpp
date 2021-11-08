/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011 Andrey Stroganov <savthe@gmail.com>

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

#ifndef SHARE_HASHER_HPP
#define SHARE_HASHER_HPP

#include <boost/noncopyable.hpp>

#include "asynchash.hpp"
#include "stored_file.hpp"

namespace share_details {

class Storage;
class LocalQht;

class Hasher: public boost::noncopyable
{
public:
	Hasher(Storage&, LocalQht&);
	~Hasher();
	void run(const std::string&);
	std::string path() const { return path_; };
	bool active() const { return ahash_.active(); }
	void stop() { ahash_.cancel(); }
	void rate(file_offset_t r) { rate_ = r * 1024 * 1024; }

private:
	void done(const boost::system::error_code&);

	AsyncHash ahash_;
	Storage& storage_;
	LocalQht& qht_;
	std::string path_;
	file_offset_t rate_;
};

} // namespace share_details

#endif //SHARE_HASHER_HPP

