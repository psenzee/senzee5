// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _FORMAT_H
#define _FORMAT_H

#include <stdarg.h>
#include <string>

namespace format
{

char        *create_format_buffer_args(const char *fmt, va_list args);
char        *create_format_buffer(const char *fmt, ...);
std::string  format_args(const char *fmt, va_list args);
std::string  format(const char *fmt, ...);

}

#endif  /** _FORMAT_H */