// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _PAIRUTIL_H
#define _PAIRUTIL_H

#include <assert.h>

/** PairUtil.<p>
 *
 *  PairUtil provides static methods
 *  to manipulate Pair(s).<p>
 *
 *  @author     Paul D Senzee
 *  @version    0.5
 *
 */

#include "pair.h"

#include <vector>
#include <stdarg.h>

class PairIter
{
public:

    inline PairIter(Pair *pair = 0) : iter(pair) {}
    inline PairIter(PairIter &pi) : iter(pi.iter) {}

    inline PairIter &operator=(const PairIter &i)        { iter = i.iter; return *this; }

    inline bool      operator==(const PairIter &i) const { return iter == i.iter; }
    inline bool      operator!=(const PairIter &i) const { return iter != i.iter; }
    inline PairIter &operator++()                        { Object *t = iter->tail; iter = t ? t->cast_pair() : 0; return *this; }
    inline Object *  operator*()                         { return iter->head; }

    Pair *iter;
};

class PairList
{
public:

    Pair     *list, *tail;
    unsigned  count;

    inline PairList() : list(0), tail(0), count(0) {}

    void add(Object *obj);
    void destructive_append(Pair *pair);
    void append(Pair *pair);
};

#define __pair_inline inline

class PairUtil
{
public:

    __pair_inline static Pair         *pair(Object *a, Object *b)                 { return _gc_new(new Pair(a, b)); }
    __pair_inline static Pair         *cast_pair(Object *e)                       { return e ? e->cast_pair() : 0;  }
    __pair_inline static const Pair   *cast_pair(const Object *e)                 { return e ? e->cast_pair() : 0;  }
	__pair_inline static Object       *head(Object *e)                            { Pair *p = cast_pair(e); return p ? p->head : 0; }
    __pair_inline static const Object *head(const Object *e)                      { const Pair *p = cast_pair(e); return p ? p->head : 0; }
    __pair_inline static void          sethead(Object *e, Object *u)              { Pair *p = cast_pair(e); if (p) p->head = u; }
    __pair_inline static Object       *tail(Object *e)                            { Pair *p = cast_pair(e); return p ? p->tail : 0; }
    __pair_inline static const Object *tail(const Object *e)                      { const Pair *p = cast_pair(e); return p ? p->tail : 0; }
    __pair_inline static void          settail(Object *e, Object *u)              { Pair *p = cast_pair(e); if (p) p->tail = u; }
    __pair_inline static void          set(Object *e, Object *head, Object *tail) { Pair *p = cast_pair(e); if (p) { p->head = head; p->tail = tail; } }
    __pair_inline static Pair         *destructive_append(Pair *a, Pair *b)       { Pair *p = lastpair(a, 0); p->tail = b; }
    __pair_inline static Pair         *append(Pair *a, Object *b)                 { return !a ? _pairify(b) : _append(a, b); }
    __pair_inline static Pair         *append(Object *a, Object *b)               { if (!a) return _pairify(b); if (!b) return _pairify(a); Pair *pa = cast_pair(a); return pa ? _append(pa, b) : pair(a, b); }
	__pair_inline static bool          isatom(const Object *e)                    { return !cast_pair(e); }
	__pair_inline static bool          ispair(const Object *e)                    { return !!cast_pair(e); }
	__pair_inline static bool          islist(const Object *e)                    { return !e || cast_pair(e); }
	__pair_inline static bool          isassoc(const Object *e)                   { return cast_pair(e) && !isproperlist(e); }
    __pair_inline static unsigned      length(const Pair *p)                      { unsigned count = 0; lastpair(p, &count); return count; }
    __pair_inline static Object       *last(Pair *p)                              { p = lastpair(p); return p ? p->head : 0; }
    __pair_inline static Object       *copylist(Object *a)                        { if (!a) return 0; Pair *pa = cast_pair(a); return pa ? copylist(pa) : a; }
    __pair_inline static Object       *copytree(Object *a)                        { if (!a) return 0; Pair *pa = cast_pair(a); return pa ? copytree(pa) : a; }
    __pair_inline static Object       *at(Pair *p, unsigned n)                    { p = nthpair(p, n); return p ? p->head : 0; }
    __pair_inline static void          set(Pair *p, unsigned n, Object *e)        { p = nthpair(p, n); if (p) p->head = e; }
    __pair_inline static const Pair   *lastpair(const Pair *a, unsigned *count)   { return lastpair(const_cast<Pair *>(a), count); }
    __pair_inline static unsigned      copy(Object *e, std::vector<Object *> &v)  { if (!e) return 0; Pair *p = cast_pair(e); if (p) return copy(p, v); v.push_back(e); return 1; }

	static bool                 isproperlist(const Object *e);
    static Pair                *findassoc(Object *list, Object *e);
    static Pair                *nthpair(Pair *a, unsigned n);
    static Pair                *lastpair(Pair *a, unsigned *count = 0);
    static Pair                *list(Object **values, unsigned count);
    static Pair                *reverselist(Object **values, unsigned count);
	static Pair                *reverse(Pair *p);
    static Object              *duplicate(Object *item, unsigned count);
    static Pair                *copylist(Pair *a, Pair **end = 0, unsigned *count = 0);
    static Pair                *copytree(Pair *a, Pair **end = 0, unsigned *count = 0);
    static unsigned             copy(Pair *p, std::vector<Object *> &v);
    static IOutStream          &print(IOutStream &os, const Object *e, unsigned level = 0, bool pretty = true, bool indent_all = false);

    static Pair                *list_args(unsigned count, va_list args);
    static Pair                *list(unsigned count, ...);

private:
   
    inline static Pair         *_pairify(Object *a)                        { if (!a) return 0; Pair *p = cast_pair(a); return p ? p : pair(p, 0); }
    inline static Pair         *_append(Pair *a, Object *b)                { Pair *e = 0, *p = copylist(a, &e); if (e) e->tail = b; return p; }

    PairUtil();
    ~PairUtil();
};

#endif  // _PAIRUTIL_H