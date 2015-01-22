// From Senzee 5 blog post: Formatting std::string
//
// Paul Senzee
//
// The following snippet has been a part of my personal code library for years. It's useful for formatting a 
// std::string in a traditional printf() way. For all its ills, printf/sprintf() is incredibly convenient. 
// This code is for Win32. Minor  modification is required for Unix.

#include <stdio.h>
#include <stdarg.h>
#include <string>

std::string format_arg_list(const char *fmt, va_list args)
{
    if (!fmt) return "";
    int   result = -1, length = 256;
    char *buffer = 0;
    while (result == -1)
    {
        if (buffer) delete [] buffer;
        buffer = new char [length + 1];
        memset(buffer, 0, length + 1);
        result = _vsnprintf(buffer, length, fmt, args);
        length *= 2;
    }
    std::string s(buffer);
    delete [] buffer;
    return s;
}

std::string format(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::string s = format_arg_list(fmt, args);
    va_end(args);
    return s;
}