// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _COLLECTION_H
#define _COLLECTION_H

#include "object.h"
#include "types.h"
#include "iterator.h"

class Collection : public Iterable
{
public:

    virtual unsigned   length()               const = 0;
    virtual Object    *get(unsigned i)              = 0;
    virtual Object    *remove(unsigned i)           = 0;
    virtual void       set(unsigned i, Object *u)   = 0;
    virtual void       add(Object *u)               = 0;
    virtual void       append(Iterator *u)          = 0;

    Collection        *cast_collection()       { return this; }
    const Collection  *cast_collection() const { return this; }
};

#endif  // _COLLECTION_H