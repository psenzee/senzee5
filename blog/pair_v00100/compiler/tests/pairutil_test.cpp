// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "pairutil_test.h"

#include "pairutil.h"
#include "object.h"
#include "numbert.h"
#include "pair.h"
#include "test_utils.h"

#include "strings.h"
#include "streams.h"
#include "types.h"

Pair *make_symbol_list(const char **array, unsigned count)
{
    Object **objects = new Object * [count];
    for (unsigned i = 0; i < count; i++)
        objects[i] = Types::create_symbol(array[i]);
    Pair *p = PairUtil::list(objects, count);
    delete [] objects;
    return p;
}

#define COUNT(X) (sizeof((X)) / sizeof((X)[0]))
#define LIST(X)  (make_symbol_list(X, COUNT(X)))

void test_lastpair(Pair *p)
{
    printf("-\n");
    printf("lastpair - in:    ");
    println_console(p);
    unsigned count = 0;
    printf("lastpair - out:   ");
    println_console(PairUtil::lastpair(p, &count));
    printf("lastpair - count: %u\n", count);
}

void test_nthpair(Pair *p)
{
    printf("-\n");
    printf("nthpair  - in:    ");
    println_console(p);
    printf("nthpair  - out:   \n");
    for (unsigned i = 0, len = PairUtil::length(p) + 2; i < len; i++)
    {
        printf("pair %u: ", i);
        println_console(PairUtil::nthpair(p, i));
    }
}

void test_copylist(Pair *p)
{
    printf("-\n");
    printf("copylist - in:    ");
    println_console(p);
    printf("copylist - out:   ");
	Pair *pend = 0;
    print_console(PairUtil::copylist(p, &pend));
    printf(" (end = ");
    print_console(pend);
    printf(")\n");
}

void test_reverse(Pair *p)
{
    printf("-\n");
    printf("reverse  - in:    ");
    println_console(p);
    printf("reverse  - out:   ");
    println_console(PairUtil::reverse(p));
}

void test_copytree(Pair *p)
{
    printf("-\n");
    printf("copytree - in:    ");
    println_console(p);
    printf("copytree - out:   ");
    println_console(PairUtil::copytree(p));
}

void test_append(Pair *p, Pair *q)
{
    p = PairUtil::copylist(p, 0);
    q = PairUtil::copylist(q, 0);
    printf("-\n");
    printf("append   - in:    \n");
    println_console(p);
    println_console(q);
    Pair *concat = PairUtil::append(p, q);
    unsigned psz = PairUtil::length(p), qsz = PairUtil::length(q), pn  = psz / 2, qn = psz + qsz / 2;
    printf("append   - modifying %uth and %uth elements\n", pn, qn);
    Pair *pelem = PairUtil::nthpair(concat, pn), *qelem = PairUtil::nthpair(concat, qn);
    if (pelem) pelem->head = Types::create_symbol("*");
    if (qelem) qelem->head = Types::create_symbol("@");
    printf("append   - out:   ");
    println_console(concat);
    printf("append   - in*:   \n");
    println_console(p);
    println_console(q);
}

void pairutil_test()
{
    const char *syms1[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };
    Pair       *list0   = LIST(syms1),
               *dotted0 = PairUtil::pair(Types::create_symbol("a"), Types::create_symbol("b")),
               *nil     = 0;

    printf("\n-- PairUtil tests --\n");
    test_lastpair(list0);
    test_lastpair(dotted0);
    test_lastpair(nil);
    test_copylist(list0);
    test_copylist(dotted0);
    test_copylist(nil);
    test_reverse(list0);
    test_reverse(dotted0);
    test_reverse(nil);
    test_copytree(list0);
    test_copytree(dotted0);
    test_copytree(nil);
    test_append(list0, list0);
    test_append(list0, nil);
    test_append(list0, dotted0);
    test_append(nil, list0);
    test_append(nil, nil);
    test_append(dotted0, nil);
    test_append(dotted0, list0);
    test_append(dotted0, dotted0);
    test_nthpair(list0);
    test_nthpair(dotted0);
    test_nthpair(nil);

    printf("\n-- PairIter tests --\n");

    printf("\n-- PairList tests --\n");

    wait_console();
}