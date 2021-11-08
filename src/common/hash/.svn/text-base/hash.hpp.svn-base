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

#ifndef HASH_HPP
#define HASH_HPP

#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>


struct HashConversionError: public std::runtime_error
{
	HashConversionError(): std::runtime_error("Hash conversion error") {}
};

template 
<
	class HashType,
	int hash_size, 
	class Converter
> 
class Hash 
{
public:

	template <class HT, int h_s, class Cnv> 	
	friend bool operator==(const Hash<HT, h_s, Cnv>& h1, const Hash<HT, h_s, Cnv>& h2);
	
	template <class HT, int h_s, class Cnv> 	
	friend bool operator!=(const Hash<HT, h_s, Cnv>& h1, const Hash<HT, h_s, Cnv>& h2);
	
	template <class HT, int h_s, class Cnv> 	
	friend bool operator<(const Hash<HT, h_s, Cnv>& h1, const Hash<HT, h_s, Cnv>& h2);

	typedef char* iterator;
	typedef const char* const_iterator;

	Hash() { clear(); }

	template <typename InputIterator>
	Hash(InputIterator iBegin) { assign(iBegin); }
	Hash(const Hash<HashType, hash_size, Converter>& h) { assign(h.begin()); }
	Hash( const std::string& s ) { fromString( s ); }

	iterator begin() { return hash_; }
	const_iterator begin() const { return hash_; }
	iterator end() { return hash_ + hash_size; }
	const_iterator end() const { return hash_ + hash_size; }

	bool empty() const 
	{ 
		for(std::size_t i = 0; i < hash_size; ++i)
			if(hash_[i])
				return false;
		return true;
	}

	void clear() 
	{ 
		std::fill(hash_, hash_ + hash_size, 0);
	}

	static std::size_t size() { return hash_size; }
	const char* bytes() const { return hash_; }
	
	template <typename T>
	void assign(T iBegin)
	{
		for(int i = 0; i < hash_size; ++i, ++iBegin)
			hash_[i] = *iBegin;
	}

	Hash<HashType, hash_size, Converter>& operator= (const Hash<HashType, hash_size, Converter>& h) 
	{
		if(this != &h) 
			assign(h.begin());
		
		return *this;	
	}
	
	std::string toString() const
	{
		std::string result;
		try
		{
			Converter::encode(begin(), end(), std::back_inserter(result));
		}
		catch(...)
		{
			throw HashConversionError();
		}
		return result;
	}

	void fromString(const std::string& str)
	{
		std::vector <char> v;
		try
		{
			Converter::decode(str.begin(), str.end(), std::back_inserter(v));
		}
		catch(...)
		{
			throw HashConversionError();
		}
		
		if(v.size() != hash_size) throw HashConversionError();

		assign(v.begin());
	}
	
	
protected:
	char hash_[hash_size];
};	

template <class HashType, int hash_size, class Converter> 
bool operator==(const Hash<HashType, hash_size, Converter>& h1, const Hash<HashType, hash_size, Converter>& h2)
{
	return std::equal(h1.begin(), h1.end(), h2.begin());
}

template <class HashType, int hash_size, class Converter> 
bool operator!=(const Hash<HashType, hash_size, Converter>& h1, const Hash<HashType, hash_size, Converter>& h2)
{
	return !(h1 == h2);
}

template <class HashType, int hash_size, class Converter> 
bool operator<(const Hash<HashType, hash_size, Converter>& h1, const Hash<HashType, hash_size, Converter>& h2)
{
	return std::lexicographical_compare(h1.begin(), h1.end(), h2.begin(), h2.end());
}

#endif //HASH_HPP
