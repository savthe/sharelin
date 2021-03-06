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

#include <assert.h>
#include <sstream>
#include <iostream>

#include "ip.hpp"

namespace ip {

bool Address::fromString(const std::string& sip)
{
	std::string tmpStr(sip);

	for(std::size_t i = 0; i < tmpStr.size(); ++i)
                if(tmpStr[i] == '.') tmpStr[i] = ' ';

	clear();

        std::stringstream s(tmpStr);
        uint a, b, c, d;
        if((s >> a >> b >> c >> d) && a <= 255 && b <= 255 && c <= 255 && d <= 255)
	{
		char x;
		if(s >> x) return false;
                value = (d << 24) | (c << 16) | (b << 8) | a; 
		return true;
	}

	return false;
}

std::string Address::toString() const
{
        std::stringstream s;
	s << (value & 0x000000FF) << "." << ((value & 0x0000FF00) >> 8) << "." << ((value & 0x00FF0000) >> 16) << "." << (value >> 24);
        return s.str();
}

uint32 Address::reversed() const
{
	return ((value << 24) & 0xFF000000) + ((value << 8) & 0x00FF0000) + ((value >> 8) & 0x0000FF00) + (value >> 24);
}

void Address::reverse()
{
	value = ((value << 24) & 0xFF000000) + ((value << 8) & 0x00FF0000) + ((value >> 8) & 0x0000FF00) + (value >> 24);
}

bool Endpoint::fromString(const std::string& ep)
{
	std::string tmpStr(ep);
	std::string::size_type i = tmpStr.find(':');
        if(i != std::string::npos)
        {
                tmpStr[i] = ' ';
                std::stringstream s(tmpStr);       
                std::string strIp;
                if(s >> strIp >> port)
			return address.fromString(strIp); 
	}

        return false; 
}

std::string Endpoint::toString() const
{
	std::stringstream s;
        s << address.toString() << ":" << port;
	return s.str();
}

bool operator< (const Address& ip1, const Address& ip2)
{
	return ip1.value < ip2.value;
}

bool operator< (const Endpoint& ep1, const Endpoint& ep2)
{
	if(ep1.address < ep2.address) return true;
	if(ep1.address != ep2.address) return false;
	return ep1.port < ep2.port;
}

bool operator== (const Address& ip1, const Address& ip2)
{
	return ip1.value == ip2.value;
}

bool operator!= (const Address& ip1, const Address& ip2)
{
	return !(ip1 == ip2);
}

bool operator== (const Endpoint& ep1, const Endpoint& ep2)
{
	return (ep1.address == ep2.address) && (ep1.port == ep2.port);
}

bool operator!= (const Endpoint& ep1, const Endpoint& ep2)
{
	return !(ep1 == ep2);
}

} //namespace ip
