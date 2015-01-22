// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "numeric_to_string.h"

#include <string>
#include <string.h>

namespace format
{

void numeric_to_string(__int64 v,          std::string &s) { char sb[32]; sprintf(sb, "%I64d", v); s = sb; }
void numeric_to_string(unsigned __int64 v, std::string &s) { char sb[32]; sprintf(sb, "%I64u", v); s = sb; }
void numeric_to_string(double v,           std::string &s) { char sb[32]; sprintf(sb, "%.02f", v); s = sb; }
void numeric_to_string(float v,            std::string &s) { numeric_to_string((double)v, s); }
void numeric_to_string(int v,              std::string &s) { char sb[32]; sprintf(sb, "%d", v); s = sb; }
void numeric_to_string(unsigned int v,     std::string &s) { char sb[32]; sprintf(sb, "%u", v); s = sb; }
void numeric_to_string(short v,            std::string &s) { numeric_to_string((int)v, s); }
void numeric_to_string(unsigned short v,   std::string &s) { numeric_to_string((unsigned int)v, s); } 
void numeric_to_string(char v,             std::string &s) { numeric_to_string((int)v, s); }
void numeric_to_string(unsigned char v,    std::string &s) { numeric_to_string((unsigned int)v, s); }

}