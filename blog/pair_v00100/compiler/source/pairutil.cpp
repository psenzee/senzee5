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

Pair *PairUtil::list_args(unsigned count, va_list args)
{
    PairList list;
    for (unsigned i = 0; i < count; i++)
        list.add(va_arg(args, Object *));
    return list.list;
}

Pair *PairUtil::list(unsigned count, ...)
{
    va_list args;
    va_start(args, count);
    Pair *p = list_args(count, args);
    va_end(args);
    return p;
}

Pair *PairUtil::findassoc(Object *list, Object *e)
{
    Pair *p = cast_pair(list);
    while (p)
    {
        Pair *assoc = cast_pair(p->head);
        if (assoc && Object::equal(e, assoc->head))
            return assoc;        
        p = cast_pair(p->tail);
    }
    return 0;   
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

#include "strings.h"

static bool has_function_symbol(const Object *e, const char *fnname)
{
    const Pair   *p = Object::cast_pair(e);
    const Symbol *s = p ? Object::cast_symbol(p->head) : 0;
    return s && s->str() == fnname;
}

bool is_string_literal(const Object *e)
{
    return Object::cast_string(e) != 0;
}

static std::string _escape(const std::string &s)
{
    std::string out;
    for (std::string::const_iterator i = s.begin(), e = s.end(); i != e; ++i)
    {
        char c = *i;
        switch (c)
        {
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        case '\f': out += "\\f"; break;
        default:   out += c;     break;
        }
    }
    return out;
}

void write_string_literal(IOutStream &os, const Object *e)
{
    write(os, "\"");
    write(os, _escape(Object::str(e)));
    write(os, "\"");
}

void write_quote_literal(IOutStream &os, const Object *e)
{
    const Pair *p = Object::cast_pair(e);
    if (p && (p = Object::cast_pair(p->tail)) != 0)
    {
        write(os, "'");
        PairUtil::print(os, p->head, 0, false);
    }
}

IOutStream &PairUtil::print(IOutStream &os, const Object *e, unsigned level, bool pretty, bool indent_all)
{
    const Pair *pe = 0;

    bool newline_bracket = false;

    if (pretty && level && (indent_all || cast_pair(e) != 0))
    {
        if ((newline_bracket = has_function_symbol(e, "function")))
            write(os, "\n");
        write(os, "\n");
        for (unsigned i = 0; i < level; i++)
            write(os, " ");
    }

    if (!e)
        write(os, "()");
    else if (is_string_literal(e))
        write_string_literal(os, e);
    else if (has_function_symbol(e, "quote"))
        write_quote_literal(os, e);
    else if ((pe = cast_pair(e)) != 0)
    {
        bool          first = true;
        const Object *et    = 0;
        write(os, "(");
        do
        {
            if (!first) write(os, " ");

            print(os, head(pe), level + 1, pretty, indent_all);

            first = false;
            et = tail(pe);
            if (islist(et))
                pe = (Pair *)et;
            else
            {
                write(os, " . ");
                print(os, et, 0, false);
                pe = 0;
            }
        }
        while (pe);

        write(os, ")");
    }
    else
        write(os, Object::str(e));

    if (newline_bracket)
        write(os, "\n");

    return os;
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