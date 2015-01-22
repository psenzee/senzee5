// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "read_type.h"

unsigned int read_chain_uint(IInStream &s)
{
    int     n = 0;
    __int64 v = 0;
    while ((n = read_type<unsigned char>(s)) & 0x80)
        v = (v << 7) | (n & 0x7f);
    return (int)((v << 7) | n);
}