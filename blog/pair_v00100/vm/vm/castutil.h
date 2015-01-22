// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _CASTUTIL_H
#define _CASTUTIL_H

#include "object.h"
#include "pair.h"
#include "iterator.h"
#include "pairiterator.h"
#include "exceptions.h"

#include <vector>

class CastUtil
{
public:

    static Number *number(Object *r)
    {
        Number *n = 0;
        if (r)
        {
            n = r->cast_number();
            if (!n)
                throw RuntimeException("Can't convert object to number.\n");
        }
        return n;
    }

private: 

    CastUtil() {}
};

#endif  // _CASTUTIL_H