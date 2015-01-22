// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "test_utils.h"

#include "pairutil.h"
#include "object.h"
#include "streams.h"

#include <stdio.h>
#include <conio.h>

StdOutStream g_test_stdout(stdout);

void wait_console()
{
    printf("\nHit any key..\n");
    getch();
}

void print_console(const Object *e)
{
    PairUtil::print(g_test_stdout, e);
    const Pair *p = PairUtil::cast_pair(e);
    if (p) printf(" [%u]", PairUtil::length(p));
}

void println_console(const Object *e)
{
    print_console(e);
    printf("\n");
}

void clear_console(unsigned lines)
{
    for (unsigned i = 0; i < lines; i++)
        printf("\n");
}