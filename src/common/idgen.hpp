/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2009 Andrey Stroganov <savthe@gmail.com>

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

#ifndef IDGEN_HPP
#define IDGEN_HPP

#include "types.hpp"
#include "increment.hpp"

namespace Misc
{
class IdGenerator
{
public:
	IdGenerator(): gen_(1) {}
	IdGenerator(Id id): gen_(id) {}
	Id get() 
	{
		Id id;
		while((id = gen_.get()) == 0);
		return id;
	}

private:
	Increment<Id> gen_;
};

Id genId();

} //namespace Misc

#endif //IDGEN_HPP

