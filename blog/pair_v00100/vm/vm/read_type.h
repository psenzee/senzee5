// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _READ_TYPE_H
#define _READ_TYPE_H

#include "streams.h"

template <typename T> 
inline T read_type(IInStream &s)
{ 
    T v = 0;
    s.read(&v, sizeof(T));
    return v;
}

#include <string>

template <typename T, typename StrT>
std::basic_string<StrT> read_prefix_str(IInStream &s)
{
    static StrT empty[] = { 0 };
    T sz = read_type<T>(s);
    if (!sz) return empty;
    StrT *str = new StrT [sz + 1];
    str[sz] = 0;
    s.read(str, (int)(sz * sizeof(StrT)));
    std::basic_string<StrT> stds(str, (size_t)sz);
    delete [] str;
    return stds;
}

unsigned int read_chain_uint(IInStream &s);

#endif  // _READ_TYPE_H