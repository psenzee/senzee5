// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "exceptional.h"
#include "format.h"
#include <string>

void throw_exception(throw_exception_fn_t throw_fn, const char *message_format, ...)
{
    va_list args;
    va_start(args, message_format);
    std::string s(format::format_args(message_format, args));
    va_end(args);
    throw_fn(s.c_str());
}