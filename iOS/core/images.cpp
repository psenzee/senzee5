#include "images.h"

#include <stdio.h>
#include <string.h>

void Flip(unsigned char *data, int width, int height, int bytesPerPixel)
{
    int stride = width * bytesPerPixel;
    unsigned char *scanline = new unsigned char [stride];
    for (int i = 0; i < height / 2; i++)
    {
        memcpy(scanline, data + i * stride, stride);
        memcpy(data + i * stride, data + (height - i - 1) * stride, stride);
        memcpy(data + (height - i - 1) * stride, scanline, stride);
    }
    delete [] scanline;
}

bool HasAlphaChannelRGBA32(const unsigned char *data, int width, int height)
{
	enum { VALUES = 256, BYTES_PER_PIXEL = 4, ALPHA_START = 3 };
	int counts[VALUES];
	memset(counts, 0, sizeof(int) * VALUES);
	int stride = width * BYTES_PER_PIXEL;
	for (int i = ALPHA_START; i < stride * height; i += BYTES_PER_PIXEL)
		counts[data[i]]++;
	int countOfCounts = 0;
	for (int i = 0; i < VALUES; i++)
		if (counts[i] != 0) countOfCounts++;
	if (countOfCounts == 1)
		return false;
	return true;
}

void ConvertRGBA8888ToRGB565(unsigned char *data, int width, int height)
{
	enum { BYTES_PER_PIXEL_IN = 4, BYTES_PER_PIXEL_OUT = 2 };	
	const unsigned char *pin = data, *pend = data + BYTES_PER_PIXEL_IN * width * height;
	unsigned char *pout = data;	
	for (; pin < pend; pin += BYTES_PER_PIXEL_IN, pout += BYTES_PER_PIXEL_OUT)
		ConvertPixelRGBA8888ToRGB565(pin, pout);
}
