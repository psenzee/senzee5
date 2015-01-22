// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _ITERATOR_H
#define _ITERATOR_H

#include "object.h"
#include "types.h"

class Iterable : public GcObject
{
public:

    virtual Iterator *iterator() = 0;

    Iterable        *cast_iterable()       { return this; }
    const Iterable  *cast_iterable() const { return this; }
};

class Iterator : public Iterable
{
public:

    virtual bool          hasnext() = 0;
    virtual Object       *next()    = 0;    

    Iterator             *iterator()                            { return this; }

    Iterator             *cast_iterator()                       { return this; }
    const Iterator       *cast_iterator()                 const { return this; }
    const Type           &type()                          const { return type_iterator; }
};

class NullIterator : public Iterator
{
public:
    bool    hasnext() { return false; }
    Object *next()    { return 0; }

    static NullIterator instance;
};

#endif  // _ITERATOR_H