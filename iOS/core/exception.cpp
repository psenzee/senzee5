#include "exception.h"

#include <string>
#include <exception>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef WIN32
#include <windows.h>

#include "format.h"

void _throw_exception(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::string s(format::format_args(fmt, args));
    va_end(args);
    fprintf(stderr, "(throw) %s\n", s.c_str());
    ::OutputDebugStr(s.c_str());
//  __asm int 3;
    throw std::exception(s.c_str());
}
#else
void _throw_exception(const char *fmt, ...)
{
}
#endif
