// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "sizes_test.h"

#include <stdio.h>
#include <conio.h>

#define PRINT_SIZE(X) printf("%-20s [size] = %u\n", #X, sizeof(X))

#include "object.h"
#include "pair.h"
#include "pairiterator.h"
#include "strings.h"
#include "numbert.h"
#include "userobject.h"
#include "iterator.h"
#include "gc.h"

void sizes_test()
{
    printf("- abstract -\n");
    PRINT_SIZE(ICollectable);
    PRINT_SIZE(Object);
    PRINT_SIZE(GcObject);
    PRINT_SIZE(TrackedObject);
    PRINT_SIZE(PlainObject);
    PRINT_SIZE(Iterator);
    printf("- concrete -\n");
    PRINT_SIZE(Type);
    PRINT_SIZE(Pair);
    PRINT_SIZE(NumberT<float>);
    PRINT_SIZE(NumberT<double>);
    PRINT_SIZE(NumberT<int>);
    PRINT_SIZE(String);
    PRINT_SIZE(Symbol);
    PRINT_SIZE(Character);
    PRINT_SIZE(PairIterator);
    PRINT_SIZE(StringIterator);
    PRINT_SIZE(UserObject);
    getch();
}

#undef PRINT_SIZE