// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _NUMERIC_TO_STRING_H
#define _NUMERIC_TO_STRING_H

#include <string>

namespace format
{

extern void numeric_to_string(__int64 v,          std::string &s);
extern void numeric_to_string(unsigned __int64 v, std::string &s);
extern void numeric_to_string(double v,           std::string &s);
extern void numeric_to_string(float v,            std::string &s);
extern void numeric_to_string(int v,              std::string &s);
extern void numeric_to_string(unsigned int v,     std::string &s);
extern void numeric_to_string(short v,            std::string &s);
extern void numeric_to_string(unsigned short v,   std::string &s);
extern void numeric_to_string(char v,             std::string &s);
extern void numeric_to_string(unsigned char v,    std::string &s);

}

#endif  // _NUMERIC_TO_STRING_H