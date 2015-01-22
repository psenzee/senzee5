// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _C_STR_H
#define _C_STR_H

#include <string>

inline const char *c_str(const char *sz)        { return sz; }
inline const char *c_str(const std::string &s)  { return s.c_str(); }

#endif  // _C_STR_H