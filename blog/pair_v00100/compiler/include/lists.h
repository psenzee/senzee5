// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _LISTS_H
#define _LISTS_H

#include "pairutil.h"

template <typename T>
void reverse(T *list, unsigned count)
{
    T         t;
    unsigned  last = count - 1;
    for (unsigned i = 0, sz = count >> 1; i < sz; i++)
    {
        t = list[i];
        list[i] = list[last - i];
        list[last - i] = t;
    }
}

template <typename ArrayT>
Pair *list_array(ArrayT &array, unsigned count, Pair **end)
{
    Pair *list = 0, *last = 0;
    for (unsigned i = 0; i < count; i++)
    {
        Pair *p = PairUtil::pair(array[i], 0);
        if (last) last->tail = p;
        last = p;
        if (!list) list = p;
    }
    if (end) *end = last;
    return list;
}

template <typename IteratorT>
Pair *list_iterator(IteratorT &i, IteratorT &e, Pair **end)
{
    Pair *list = 0, *last = 0;
    for (; i != e; ++i)
    {
        Pair *p = PairUtil::pair(*i, 0);
        if (last) last->tail = p;
        last = p;
        if (!list) list = p;
    }
    if (end) *end = last;
    return list;
}

#endif  // _LISTS_H