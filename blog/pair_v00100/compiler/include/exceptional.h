// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _EXCEPTIONAL_H
#define _EXCEPTIONAL_H

typedef void (*throw_exception_fn_t)(const char *message);

void throw_exception(throw_exception_fn_t, const char *message_format, ...);

#endif  // _EXCEPTIONAL_H