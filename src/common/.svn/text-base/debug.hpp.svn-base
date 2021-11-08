#ifndef DEBUG_HPP
#define DEBUG_HPP


#ifdef DEBUG
#include <assert.h>
#include <iostream>

#define PRINT(s) std::cout << s << std::endl;
#define PRINT2(s1, s2) std::cout << s1 << " " << s2 << std::endl;
#define PRINT3(s1, s2, s3) std::cout << s1 << " " << s2 << " " << s3 << std::endl;

template <typename T> 
void print_hex(T begin, T end)
{
	for(T i = begin; i != end; ++i)
	{
		std::cout << std::uppercase << std::hex << (int(*i) & 0xFF);
		std::cout << ' ';
	}
	std::cout << std::dec;
	std::cout << std::endl;
}

template <typename T>
void printStream(T& s)
{
	std::istream is(&s);
	std::streampos pos = is.tellg();
	while(is) std::cout << char(is.get());
	is.seekg(pos);
}
#else //DEBUG

#define PRINT(s)
#define PRINT2(s1, s2)
#define PRINT3(s1, s2, s3)

#endif //DEBUG

#endif //DEBUG_HPP

