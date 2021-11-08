/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2012 Andrey Stroganov <savthe@gmail.com>

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

#ifndef DL_VALIDATOR_HPP
#define DL_VALIDATOR_HPP

#include <string>

#include "tigertree.hpp"
#include "byterange.hpp"
#include "asynchash.hpp"
#include "ticker.hpp"
#include "sharedfile.hpp"

namespace download_details {

class Validator
{
public:
	Validator(SharedFile&);
	void start();
	void stop();
	bool busy() const { return ahash_.active(); }

private:
	ByteRange select_range() const;
	void validate();
	void on_hashed(const boost::system::error_code&);
	void tick();

	SharedFile& file_;
	AsyncHash ahash_;
	Ticker ticker_;
};

} // namespace download_details

#endif // DL_VALIDATOR_HPP

