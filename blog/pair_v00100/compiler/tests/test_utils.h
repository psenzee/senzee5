// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _TEST_UTILS_H
#define _TEST_UTILS_H

#include <stdio.h>

class Object;

extern void wait_console();
extern void print_console(const Object *e);
extern void println_console(const Object *e);
extern void clear_console(unsigned lines);

#endif  // _TEST_UTILS_H