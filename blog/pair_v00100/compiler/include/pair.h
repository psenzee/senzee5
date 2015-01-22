// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _PAIR_H
#define _PAIR_H

#include "object.h"

class Pair : public TrackedObject
{
public:

    mutable Object *head;
    Object         *tail;

    Pair(Object *head = 0, Object *tail = 0);

    IOutStream          &print(IOutStream &os)         const;

    Iterator            *iterator();
    Pair                *cast_pair()                         { return this; }
    const Pair          *cast_pair()                   const { return this; }
    const Type          &type()                        const;

    void                 apply(ICollectableVisitor *v) const;

    bool                 marked()                      const;
    void                 mark()                        const;
    void                 unmark()                      const;

    int                  length()                      const;
    Object              *at(int i);
};

#endif  // _PAIR_H