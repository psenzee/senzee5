// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "pairutil.h"

#include "object.h"
#include "numbert.h"
#include "pair.h"
#include "streams.h"

bool PairUtil::isproperlist(const Object *e)
{
	// verifies that the list (but not the tree) is not a dotted pair 
	if (!e) return true;
	while (e && cast_pair(e)) e = tail(e);
	return !e;
}

unsigned PairUtil::copy(Pair *p, std::vector<Object *> &v)
{
    unsigned c = 0;
    while (p)
    {
        v.push_back(p->head);
        p = cast_pair(p->tail);
        c++;
    }
    return c;
}

Pair *PairUtil::lastpair(Pair *a, unsigned *count)
{
    Pair *b = a;
    unsigned c = 0;
    while (a)
    {
        b = a;
        a = cast_pair(a->tail);
        c++;
    }
    if (count)
        *count = c;
    return b;
}

Pair *PairUtil::nthpair(Pair *a, unsigned n)
{
    for (unsigned i = 0; a && i < n; i++)
        a = cast_pair(a->tail);
    return a;
}

Pair *PairUtil::list(Object **values, unsigned count)
{
    Pair *list = 0, *last = 0;
    for (unsigned i = 0; i < count; i++)
    {
        Pair *p = pair(values[i], 0);
        if (last) last->tail = p;
        last = p;
        if (!list) list = p;
    }
    return list;
}

Pair *PairUtil::reverse(Pair *p)
{
	Pair *r = 0; Object *u = 0;
    while (p)
	{
    	r = pair(p->head, r);
		u = p->tail;
        p = cast_pair(u);
	}
	if (u && r)
		r->tail = u;
    return r;
}

Pair *PairUtil::reverselist(Object **values, unsigned count)
{
    Pair *list = 0, *last = 0;
    for (int i = (int)count - 1; i >= 0; i--)
    {
        Pair *p = pair(values[i], 0);
        if (last) last->tail = p;
        last = p;
        if (!list) list = p;
    }
    return list;
}

Object *PairUtil::duplicate(Object *item, unsigned count)
{
    Pair *list = 0, *last = 0;
    for (unsigned i = 0; i < count; i++)
    {
        Pair *p = pair(item, 0);
        if (last) last->tail = p;
        last = p;
        if (!list) list = p;
    }
    return list;
}

Pair *PairUtil::copylist(Pair *a, Pair **end, unsigned *count)
{
    Pair *p = 0, *b = 0, *t = 0;
    Object *u = 0;
    unsigned c = 0;
    while (a)
    {
        t = pair(a->head, 0);
        if (!b) b = p = t;
        else { p->tail = t; p = t; }
        a = cast_pair(u = a->tail);
        c++;
    }
    if (!a && p) p->tail = u;
	if (end)     *end = p;
    if (count)   *count = c;
    return b;
}

Pair *PairUtil::copytree(Pair *a, Pair **end, unsigned *count)
{
    Pair *p = 0, *b = 0, *t = 0;
    Object *u = 0;
    unsigned c = 0;
    while (a)
    {
        t = pair(copytree(a->head), 0);
        if (!b) b = p = t;
        else { p->tail = t; p = t; }
        a = cast_pair(u = a->tail);
        c++;
    }
    if (!a && p) p->tail = u;
    if (end)     *end = p;
    if (count)   *count = c;
    return b;
}

IOutStream &PairUtil::print(IOutStream &os, const Object *e)
{
    const Pair *pe = 0;

    if (!e)
        write(os, "NIL");
    else if ((pe = cast_pair(e)) != 0)
    {
        bool          first = true;
        const Object *et    = 0;
        write(os, "(");
        do
        {
            if (!first) write(os, " ");

            print(os, head(pe));

            first = false;
            et = tail(pe);
            if (islist(et)) pe = (Pair *)et;
            else
            {
                write(os, " . ");
                write(os, Object::str(et));
                pe = 0;
            }
        }
        while (pe);

        write(os, ")");
    }
    else
        write(os, Object::str(e));

    return os;
}

Object *PairUtil::remove(Pair *e, unsigned i)
{
    if (!i) return 0; // can't remove the zeroth element..
    Pair *p = nthpair(e, i - 1), *t = 0;
    Object *tt = 0, *th = 0;
    if (!p || !(t = Object::cast_pair(p->tail)))
        return 0;
    tt = t->tail;
    th = t->head;
    p->tail = tt;
    return th;
}

void PairList::add(Object *obj)
{
    if (obj != list || (!obj && !list))
    {
        Pair *np = PairUtil::pair(obj, 0);
        if (!list) list = tail = np;
        else
        {
            tail->tail = np;
            tail = np;
            count++;
        }
    }
}

void PairList::destructive_append(Pair *p)
{
    if (p)
    {
        if (list) tail->tail = p;
        else list = tail = p;
        unsigned c = 0;
        tail = PairUtil::lastpair(tail, &c);
        count += c;
    }
}

void PairList::append(Pair *p)
{
    if (p)
    {
        if (!list)
            list = PairUtil::copylist(p, &tail, &count);
        else
        {
            unsigned c = 0;
            Pair *t = 0;
            tail->tail = PairUtil::copylist(p, &t, &c);
            tail = t;
            count += c;
        }
    }
}