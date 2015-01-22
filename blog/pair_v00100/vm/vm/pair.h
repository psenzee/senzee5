// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _PAIR_H
#define _PAIR_H

#include "object.h"
#include "collection.h"

class Pair : public Collection
{
public:

    Object *head, *tail;    

    Pair(Object *head = 0, Object *tail = 0);

    IOutStream          &print(IOutStream &os)         const;

    Pair                *cast_pair()                         { return this; }
    const Pair          *cast_pair()                   const { return this; }
    const Type          &type()                        const;

    void                *data()                              { return (void *)&head; }

    void                 apply(ICollectableVisitor *v) const;

    // collection implementation..
    Iterator            *iterator();
    unsigned             length()                      const;
    Object              *get(unsigned i);
    void                 set(unsigned i, Object *u);
    Object              *remove(unsigned i);
    void                 add(Object *u);
    void                 append(Iterator *u);
};

#endif  // _PAIR_H