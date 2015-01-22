// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "pair.h"
#include "pairutil.h"
#include "pairiterator.h"

extern const Type type_pair;

Pair::Pair(Object *head, Object *tail) : head(head), tail(tail) {}

void Pair::apply(ICollectableVisitor *v) const // since we don't dereference during mark/unmark except in apply, we store the bit in the low bit of the head ptr
{
    if (v && v->visit(this))
    {
        Object *h = (Object *)((intptr_t)head & ~1);
        if (h)    h->apply(v);
        if (tail) tail->apply(v);
    }
}

const Type &Pair::type() const
{
    return type_pair;
}

bool Pair::marked() const
{
    return ((intptr_t)head & 1) > 0; // since we don't dereference during mark/unmark except in apply, we store the bit in the low bit of the head ptr
} 
void Pair::mark() const
{ 
    head = (Object *)((intptr_t)head |  1); // since we don't dereference during mark/unmark except in apply, we store the bit in the low bit of the head ptr
}

void Pair::unmark() const
{ 
    head = (Object *)((intptr_t)head & ~1); // since we don't dereference during mark/unmark except in apply, we store the bit in the low bit of the head ptr
}

IOutStream &Pair::print(IOutStream &os) const
{
    return PairUtil::print(os, this);
}

int Pair::length() const
{
    return PairUtil::length(this);
}

Object *Pair::at(int i)
{
    return PairUtil::at(this, i);
}

Iterator *Pair::iterator()
{
    return _gc_new(new PairIterator(this));
}