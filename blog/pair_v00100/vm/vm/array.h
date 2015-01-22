// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _ARRAY_H
#define _ARRAY_H

#include "object.h"
#include "collection.h"
#include "array_t.h"

class Array : public Collection
{
public:

    array_t m_array;

    Array(unsigned capacity = 0);
    Array(Object **values, unsigned count);
    ~Array();

    IOutStream          &print(IOutStream &os)         const;
    const Type          &type()                        const;

    void                 apply(ICollectableVisitor *v) const;

    void                 copy(Object **values, unsigned count);

    // collection implementation..
    Iterator            *iterator();
    unsigned             length()                      const;
    Object              *get(unsigned i);
    void                 set(unsigned i, Object *u);
    Object              *remove(unsigned i);
    void                 add(Object *u);
    void                 append(Iterator *u);

    void                 reverse() { ::reverse(m_array); }
};

class ArrayIterator : public Iterator
{
    Array             *m_obj;
    array_t            m_array;

public:

    inline ArrayIterator(Array *obj) : m_obj(obj), m_array(obj->m_array) {}

    void    apply(ICollectableVisitor *v) const { if (v->visit(this) && m_obj) m_obj->apply(v); }

    Object *next()    { return (Object *)*m_array.first++; }
    bool    hasnext() { return m_array.first != m_array.last; }
};

#endif  // _ARRAY_H