// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _CODEUTIL_H
#define _CODEUTIL_H

#include <iostream>
#include <string>

class CodeUtil
{
public:

    typedef unsigned char byte;

	inline static std::string code(byte a)                                 { std::string cd; cd += a;                                     return cd; }
	inline static std::string code(byte a, byte b)                         { std::string cd; cd += a; cd += b;                            return cd; }
	inline static std::string code(byte a, byte b, byte c)                 { std::string cd; cd += a; cd += b; cd += c;                   return cd; }
	inline static std::string code(byte a, byte b, byte c, byte d)         { std::string cd; cd += a; cd += b; cd += c; cd += d;          return cd; }
	inline static std::string code(byte a, byte b, byte c, byte d, byte e) { std::string cd; cd += a; cd += b; cd += c; cd += d; cd += e; return cd; }

    inline static std::string &append(std::string &c, char n)              { c += (byte)n; return c; }
    inline static std::string &append(std::string &c, unsigned char n)     { c += (byte)n; return c; }
    inline static std::string &append(std::string &c, short n)             { return append(c, (unsigned short)n); }
    inline static std::string &append(std::string &c, unsigned short n)    { c += (byte)n; n >>= 8; c += (byte)n; return c; }
    inline static std::string &append(std::string &c, int n)               { return append(c, (unsigned int)n); }
    inline static std::string &append(std::string &c, unsigned int n)      { return append(append(c, (unsigned short)n), (unsigned short)(n >> 16)); }
};

#endif  // _CODEUTIL_H
