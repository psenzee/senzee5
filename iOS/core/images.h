#ifndef _IMAGES_H
#define _IMAGES_H

#include <stdio.h>
#include <string.h>

void Flip(unsigned char *data, int width, int height, int bytesPerPixel);

inline unsigned NextPow2(unsigned v)
{
    v--;
    v |= v >> 16;
    v |= v >> 8;
    v |= v >> 4;
    v |= v >> 2;
    v |= v >> 1;
    return v + 1;
}

template <typename T>
inline bool IsPow2(T v)
{
    unsigned uv = static_cast<unsigned>(v);
    return NextPow2(uv) == uv;
}

bool HasAlphaChannelRGBA32(const unsigned char *data, int width, int height);

inline void ConvertPixelRGBA8888ToRGB565(const unsigned char *from, unsigned char *to)
{
	unsigned short pixel = ((unsigned short)(from[2] & 0xf8) >> 3) | // this mask is unnecessary, but this is not performance critical code, & I'd like consistency here then
                           ((unsigned short)(from[1] & 0xfc) << 3) |
                           ((unsigned short)(from[0] & 0xf8) << 8);
// for little endian
	to[0] = ((unsigned char *)&pixel)[0];
	to[1] = ((unsigned char *)&pixel)[1];	
// reverse for big endian, or just do a straight copy	
}

inline void ConvertPixelRGBA8888ToRGB565(const char *from, char *to)
{
	ConvertPixelRGBA8888ToRGB565((const unsigned char *)from, (unsigned char *)to);
}

void ConvertRGBA8888ToRGB565(unsigned char *data, int width, int height);

#endif // _IMAGES_H
