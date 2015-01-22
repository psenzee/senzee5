// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "array.h"
#include <string.h>

extern const Type type_array;

Array::Array(unsigned capacity)
{
    create(m_array, capacity);
}

Array::Array(Object **values, unsigned count)
{
    create(m_array, count + (count >> 1)); // count * 1.5
    copy(values, count);
}

Array::~Array()
{
    destroy(m_array);
}

void Array::copy(Object **values, unsigned count)
{
    reserve(m_array, count + (count >> 1)); // count * 1.5
    memcpy(m_array.first, values, count * sizeof(Object *));
    m_array.last = m_array.first + count;
}

IOutStream &Array::print(IOutStream &os) const
{
    return os; // for now..
}

const Type &Array::type() const
{
    return type_array;
}

void Array::apply(ICollectableVisitor *v) const
{
    for (void **p = m_array.first, **q = m_array.last; p < q; ++p)
        if (p) (*(ICollectable **)p)->apply(v);
}

Iterator *Array::iterator()
{
    return _gc_new(new ArrayIterator(this));
}

unsigned Array::length() const
{
    return m_array.last - m_array.first;
}

Object *Array::get(unsigned i)
{
    if (m_array.first + i >= m_array.last)
        return 0;
    return (Object *)m_array.first[i];
}

void Array::set(unsigned i, Object *u)
{
    if (m_array.first + i >= m_array.last)
        return; // $todo error
    m_array.first[i] = u;
}

Object *Array::remove(unsigned i)
{
    return 0; // $todo
}

void Array::add(Object *u)
{
    return ::append(m_array, u);
}

void Array::append(Iterator *u)
{
    while (u->hasnext())
        ::append(m_array, u->next());
}
