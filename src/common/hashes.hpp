/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2009  Andrey Stroganov <savthe@gmail.com>

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

#ifndef HASHES_HPP
#define HASHES_HPP

#include "hash/hash.hpp"
#include "conv/conv.hpp"

namespace hash {

namespace Mask 
{
	typedef char Type;
	const Type NONE = 0;
	const Type SHA1 = 1;
	const Type MD5 = 1 << 1;
	const Type TTR = 1 << 2;
	const Type ED2K = 1 << 3;
	const Type TT = 1 << 4;
	const Type ANY = 0xFF;
}

const std::size_t SHA1_SIZE = 20;
const std::size_t TTR_SIZE = 24;
const std::size_t MD5_SIZE = 16;
const std::size_t ED2K_SIZE = 16;
const std::size_t BP_SIZE = SHA1_SIZE + TTR_SIZE;

class BPConverter
{
public:
	template <typename InputIterator, typename OutputIterator>
	static void encode(InputIterator begin, InputIterator end, OutputIterator out)
	{
		InputIterator sep;
		for(std::size_t i = 0; sep != end && i < SHA1_SIZE; ++i, ++sep);
		assert(sep != end);

		conv::Base32::encode(begin, sep, out);
		for(std::size_t i = 0; i < SHA1_SIZE; ++i, ++out);
		*out++ = '.';

		conv::Base32::encode(sep, end, out);

#ifdef DEBUG
		for(std::size_t i = 0; i < BP_SIZE; ++i, ++begin)
			assert(begin != end);
#endif
	}

	template <typename InputIterator, typename OutputIterator>
	static void decode(InputIterator begin, InputIterator end, OutputIterator out)
	{
		InputIterator sep = begin;
		while(sep != end && *sep != '.') ++sep;
		if(sep == end || sep == begin) throw HashConversionError();
		assert(*sep == '.');

		std::vector<char> sha1;
		std::vector<char> ttr;
		conv::Base32::decode(begin, sep, std::back_inserter(sha1));
		++sep;
		conv::Base32::decode(sep, end, std::back_inserter(ttr));

		for(std::size_t i = 0; i < sha1.size(); ++i)
			*out++ = sha1[i];
		for(std::size_t i = 0; i < ttr.size(); ++i)
			*out++ = ttr[i];
	}
};
/*
template <class Converter>
class HashCoder
{
public:
	template <typename InputIterator>
	static std::string ToString(InputIterator begin, InputIterator end)
	{
		std::string result;
		Converter::Encode(begin, end, std::back_inserter(result));
		return result;
	}

	template <typename InputIterator>
	static void FromString(const std::string& s, InputIterator begin, InputIterator end)
	{
		std::vector<char> v;
		Converter::Decode(s.begin(), s.end(), std::back_inserter(v));
		for(std::size_t i = 0; i < v.size(); ++i)
		{
			if(begin == end) 
				throw Error("FromString: source too big");
			assert(begin != end);
			*begin++ = v[i];
		}

		if(begin != end) 
			throw Error("FromString: source too small");
	}
};
*/

class Base32HashConverter
{
public:
	template <typename InputIterator, typename OutputIterator>
	static void encode(InputIterator ibegin, InputIterator iend, OutputIterator iout)
	{
		conv::Base32::encode(ibegin, iend, iout, conv::Base32::compact);
	}

	template <typename InputIterator, typename OutputIterator>
	static void decode(InputIterator ibegin, InputIterator iend, OutputIterator iout)
	{
		conv::Base32::decode(ibegin, iend, iout);
	}
};
	
struct type_sha1{};
struct type_md5{};
struct type_ttr{};
struct type_bp{};
struct type_ed2k{};

typedef Hash<type_md5, MD5_SIZE, conv::Hex> MD5;
typedef Hash<type_ed2k, ED2K_SIZE, conv::Hex> ED2K;
typedef Hash<type_sha1, SHA1_SIZE, Base32HashConverter> SHA1;
typedef Hash<type_ttr, TTR_SIZE, Base32HashConverter> TTR;

class BP: public Hash<type_bp, BP_SIZE, BPConverter>
{
public:
	BP() {} 
	BP(const BP& bp)
	{
		assign(bp.begin());
	}

	BP(const SHA1& sha1, const TTR& ttr)
	{
		std::copy(sha1.begin(), sha1.end(), hash_);
		std::copy(ttr.begin(), ttr.end(), hash_ + SHA1_SIZE);
	}

	void split(SHA1& sha1, TTR& ttr) const
	{
		sha1.assign(hash_);
		ttr.assign(hash_ + SHA1_SIZE);
	}

	SHA1 getSHA1() const 
	{
		SHA1 h(hash_);
		return h;
	}

	TTR getTTR() const
	{
		TTR h(hash_ + SHA1_SIZE);
		return h;
	}

	using Hash<type_bp, BP_SIZE, BPConverter>::operator=;
};

} //namespace hash

#endif //HASHES_HPP
