// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _IO_TYPE_H
#define _IO_TYPE_H

#include "streams.h"

template <typename T> 
inline T read_type(IInStream &s)
{ 
    T v = 0;
    s.read(&v, sizeof(T));
    return v;
}

template <typename T>
inline void write_type(IOutStream &s, const T &u)
{ 
    T v = u;
    s.write(&v, sizeof(T));
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

template <typename T, typename StrT>
void write_prefix_str(IOutStream &s, const StrT *sdata, size_t sz)
{
    write_type<T>(s, sz);
    s.write(sdata, sz * sizeof(StrT));
}

template <typename T, typename StrT>
void write_prefix_str(IOutStream &s, const std::basic_string<StrT> &sdata)
{
    write_prefix_str<T, StrT>(s, sdata.data(), sdata.size());
}

void         write_chain_uint(IOutStream &s, unsigned int v);
unsigned int read_chain_uint(IInStream &s);

#endif  // _IO_TYPE_H