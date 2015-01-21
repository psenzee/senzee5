#ifndef _FIXEDBITMAP_H
#define _FIXEDBITMAP_H

#include "map2d.h"

template <int Bits>
class FixedBitMap
{
public:

    enum { BITS = Bits, BYTES = (BITS + 7) / 8 };

    unsigned width,
             height;
    char     data[BITS ];

    FixedBitMap() {}

    inline void clear(bool u = false)         { memset(data, u ? 0xff : 0, sizeof(data)); }
    inline bool get(unsigned i)         const { return (data[i >> 3] & (1 << (i & 7))) > 0; }
    inline void set(unsigned i, bool u)
    {
        unsigned char c = data[i >> 3], mask = 1 << (i & 7);
        data[i >> 3] = (u ? (c | mask) : (c & ~mask));
    }
    inline int                  size()  const { return BYTES; }
    inline unsigned char       *ptr(unsigned i)       { return data + (i >> 3); }
    inline const unsigned char *ptr(unsigned i) const { return data + (i >> 3); }
};

#endif // _FIXEDBITMAP_H