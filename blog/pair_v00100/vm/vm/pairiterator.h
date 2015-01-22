// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _PAIRITERATOR_H
#define _PAIRITERATOR_H

#include "iterator.h"
#include "pairutil.h"

class Pair;

class PairIterator : public Iterator
{
public:

    inline PairIterator(Pair *pair = 0) : m_pair(pair), m_iter(pair) {}

    void    apply(ICollectableVisitor *v) const
    {
        if (v->visit(this))
        {
            if (m_pair)      m_pair->apply(v);
            if (m_iter.iter) m_iter.iter->apply(v);
        }
    }

    bool    hasnext()                           { return m_iter.hasnext(); }
    Object *next()                              { return m_iter.next(); }

private:

	Pair     *m_pair;
    PairIter  m_iter;
};

#endif  // _PAIRITERATOR_H