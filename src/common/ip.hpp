/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008  Andrey Stroganov <gtsy@mail.ru>

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

#ifndef IP_HPP
#define IP_HPP 

#include <string>
#include "types.hpp"

namespace ip
{

namespace protocol
{
	typedef unsigned char Type;
	const Type TCP = 1;
	const Type UDP = 1 << 1;
}

struct Address
{
	Address(): value(0) {} 
	Address(uint32 v): value(v) {};

	bool empty() const { return value == 0; }
	void clear() { value = 0; }

	bool fromString(const std::string& s);
	std::string toString() const;
	uint32 reversed() const;
	void reverse();

	uint32 value;
};

struct Endpoint
{
	Endpoint(): port(0) {}
	//Endpoint(Address addr): address(addr), port(0) {}
	Endpoint(Address addr, uint16 pp): address(addr), port(pp) {}

	bool empty() const { return address.empty(); }
	bool fromString(const std::string& s);
	std::string toString() const;

	void clear()
	{
		address.clear();
		port = 0;
	}

	Address address;
	uint16 port;
};	
	
bool operator< (const Address& ip1, const Address& ip2);
bool operator< (const Endpoint& ep1, const Endpoint& ep2);

bool operator== (const Address& ip1, const Address& ip2);
bool operator!= (const Address& ip1, const Address& ip2);

bool operator== (const Endpoint& ep1, const Endpoint& ep2);
bool operator!= (const Endpoint& ep1, const Endpoint& ep2);

} //namespace ip 

template< typename Elem, typename Traits>
std::basic_ostream < Elem, Traits >& operator<<( std::basic_ostream< Elem, Traits >& os, const ip::Address& addr )
{
	os << addr.toString();
	return os;
}

template< typename Elem, typename Traits>
std::basic_ostream < Elem, Traits >& operator<<( std::basic_ostream< Elem, Traits >& os, const ip::Endpoint& ep )
{
	os << ep.toString();
	return os;
}

#endif //NETADDR_HPP


