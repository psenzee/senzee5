#ifndef _BITMAP2D_H
#define _BITMAP2D_H

#include "map2d.h"

class BitMap2d
{
public:

    unsigned             width,
                         height;
    Map2d<unsigned char> map;
    bool                 defaultValue;

    BitMap2d(unsigned width, unsigned height, bool defaultValue) : width(width), height(height), map((width + 7) / 8, height), defaultValue(defaultValue) {}

    inline void clear()                           { map.clear(); }
    inline bool get(unsigned x, unsigned y) const
    { 
        return (x >= width || y >= height) ? defaultValue 
                  : ((map.get(x >> 3, y) & (1 << (x & 7))) > 0);
    }
    inline void set(unsigned x, unsigned y, bool u)
    {
        if (x < width && y < height)
        {
            unsigned char c = map.get(x >> 3, y), mask = 1 << (x & 7);
            map.set(x >> 3, y, u ? (c | mask) : (c & ~mask));
        }
    }
    inline unsigned char       *ptr(unsigned x, unsigned y)       { return map.ptr(x >> 3, y); }
    inline const unsigned char *ptr(unsigned x, unsigned y) const { return map.ptr(x >> 3, y); }
};

#endif // _BITMAP2D_H