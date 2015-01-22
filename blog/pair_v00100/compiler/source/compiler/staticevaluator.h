// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _STATICEVALUATOR_H
#define _STATICEVALUATOR_H

#include "ievaluator.h"

class StaticEvaluator : public IEvaluator
{
    IEvaluatorMap *m_map;

public:

    StaticEvaluator(IEvaluatorMap *map) : m_map(map) {}

    static bool        is_identifier(Object *u);
    static bool        is_numeric_literal(Object *u);
    static bool        is_string_literal(Object *u);
    static bool        is_literal(Object *u);

    static bool        is_constant_true(Object *u);
    static bool        is_constant_nil(Object *u);
    static bool        is_quote_expression(Object *u);

    static bool        is_known_true(Object *u);
    static bool        is_known_false(Object *u);
    static bool        is_unknown(Object *u);

    static const char *head_identifier(Object *u);
    static Pair       *map_evaluate(IEvaluator *ev, Pair *p);
    static Pair       *remove_constants(Pair *p);
    static Pair       *remove_nonfunctions(Pair *p, bool keep_last);

    Object            *evaluate(Object *u, IEvaluator *root);

    static Object     *transform(Object *e);
};

#endif  // _STATICEVALUATOR_H