/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2010  Andrey Stroganov

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

#include <cassert>

#include "packet.hpp"

namespace g2 {

Packet::Pool Packet::pool_ = Packet::Pool();

Packet::Packet()
{
	pBuffer_ = pool_.get(); 
}

Packet::Packet(const Packet& pk)
{
	pBuffer_ = pool_.get();
	*pBuffer_ = *pk.pBuffer_;
//	pBuffer_->assign(pk.pBuffer_->data(), pk.pBuffer_->size());
}

Packet& Packet::operator=(const Packet& pk)
{
	if(&pk != this)
		*pBuffer_ = *pk.pBuffer_;

	return *this;
}

Packet::~Packet() 
{ 
	pBuffer_->clear();
	pool_.release(pBuffer_); 
}

const char* Packet::Bytes() const 
{ 
	return &((*pBuffer_)[0]); 
}

unsigned int Packet::Size() const 
{ 
	return pBuffer_->size(); 
}

Packet::Buffer* Packet::GetBuffer()
{ 
	return pBuffer_; 
}

} //namespace G2

