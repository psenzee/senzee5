// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "read_type.h"

void write_chain_uint(IOutStream &s, unsigned int v)
{
    if (!v)
    {
        write_type<unsigned char>(s, 0);
        return;
    }
    int sz = sizeof(v), sh = 28;
    __int64 mask = 0x07f0000000;
    while (mask)
    {
        if (v & mask)
            write_type<unsigned char>(s, (unsigned char)(((v & mask) >> sh) | (sh ? 0x80 : 0)));
        sh -= 7; mask >>= 7;
    }
}

unsigned int read_chain_uint(IInStream &s)
{
    int     n = 0;
    __int64 v = 0;
    while ((n = read_type<unsigned char>(s)) & 0x80)
        v = (v << 7) | (n & 0x7f);
    return (int)((v << 7) | n);
}