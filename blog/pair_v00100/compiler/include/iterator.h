// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _ITERATOR_H
#define _ITERATOR_H

#include "object.h"
#include "types.h"

class Iterator : public GcObject
{
public:

    Iterator             *cast_iterator()                       { return this; }
    const Iterator       *cast_iterator()                 const { return this; }
    const Type           &type()                          const { return type_iterator; }

    Iterator             *iterator()                            { return this; }

    virtual bool          hasnext() = 0;
    virtual Object       *next()    = 0;    
};

class NullIterator : public Iterator
{
public:
    bool    hasnext() { return false; }
    Object *next()    { return 0; }

    static NullIterator instance;
};

#endif  // _ITERATOR_H