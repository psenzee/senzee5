// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _UTIL_H
#define _UTIL_H

#include "object.h"
#include "pair.h"
#include "iterator.h"
#include "pairiterator.h"
#include "exceptions.h"

#include <vector>

class Util
{
public:

    static void reverse(Object **list, unsigned count)
    {
        Object   *t    = 0;
        unsigned  last = count - 1;
        for (unsigned i = 0, sz = count >> 1; i < sz; i++)
        {
            t = list[i];
            list[i] = list[last - i];
            list[last - i] = t;
        }
    }

private:
    Util();
};

#endif  // _UTIL_H