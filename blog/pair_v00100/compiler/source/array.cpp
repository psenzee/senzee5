// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "array.h"
#include <string.h>

void create(array_t &list, unsigned capacity)
{
    list.last = list.first = 0; list.capacity = 0;
    reserve(list, capacity);
}

void reserve(array_t &list, unsigned capacity)
{
    if (capacity > list.capacity)
    {
        // resize
        void **nl = new void * [list.capacity = capacity];
        size_t sz = list.last - list.first;
        if (list.first)
        {
            if (sz) memcpy(nl, list.first, sz * sizeof(void **));
            delete [] list.first;
        }
        list.first = nl;
        list.last  = list.first + sz;
    }
}

void reverse(array_t &list)
{
    void *t = 0;
    for (void **p = list.first, **q = list.last - 1; p < q; ++p, --q)
        { t = *p; *p = *q; *q = t; }
}

void destroy(array_t &list)
{
    if (list.first)
    {
        delete list.first;
        list.first = list.last = 0;
        list.capacity = 0;
    }    
}

void **find(array_t &list, const void *q)
{
    for (void **p = list.first, **e = list.last; p != e; ++p)
        if (*p == q)
            return p;
    return 0;
}

void append(array_t &list, void *p)
{
    if (list.last >= (list.first + list.capacity)) // reallocate
        reserve(list, list.capacity + ((list.capacity + 1) >> 1)); // capacity *= 1.5
    *list.last++ = p;
}