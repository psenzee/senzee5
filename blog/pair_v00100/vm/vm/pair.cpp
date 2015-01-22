// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "pair.h"
#include "pairutil.h"
#include "pairiterator.h"
#include "exceptions.h"

extern const Type type_pair;

Pair::Pair(Object *head, Object *tail) : head(head), tail(tail) {}

void Pair::apply(ICollectableVisitor *v) const // since we don't dereference during mark/unmark except in apply, we store the bit in the low bit of the head ptr
{
    if (v && v->visit(this))
    {
        if (head) head->apply(v);
        if (tail) tail->apply(v);
    }
}

const Type &Pair::type() const
{
    return type_pair;
}

IOutStream &Pair::print(IOutStream &os) const
{
    return PairUtil::print(os, this);
}

unsigned Pair::length() const
{
    return PairUtil::length(this);
}

Object *Pair::get(unsigned i)
{
    return PairUtil::at(this, i);
}

Iterator *Pair::iterator()
{
    return _gc_new(new PairIterator(this));
}

void Pair::set(unsigned i, Object *u)
{
    PairUtil::set(this, i, u);
}

Object *Pair::remove(unsigned i)
{
    if (i == 0) throw RuntimeException("Can't remove 0th item of a list of pairs.");
    return PairUtil::remove(this, i);
}

void Pair::add(Object *u)
{
    Pair *p = PairUtil::lastpair(this);
    if (p) p->tail = PairUtil::pair(u, 0);
}

void Pair::append(Iterator *u)
{
    if (!u) return;
    Pair *p = PairUtil::lastpair(this);
    if (p)
    {
        PairList list;
        while (u->hasnext()) list.add(u->next());  
        p->tail = list.list;
    }
}