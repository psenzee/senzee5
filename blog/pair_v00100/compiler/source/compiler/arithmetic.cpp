// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "arithmetic.h"
#include "pairutil.h"
#include "staticevaluator.h"
#include "numbert.h"
#include "types.h"

typedef NumberT<int>   int_t;
typedef NumberT<float> float_t;

inline static Object  *_float(float v)      { return Types::create_float(v); }
inline static Object  *_int(int v)          { return Types::create_int(v); }
inline static float_t *_to_float(Object *u) { Number *n = u ? u->cast_number() : 0; if (!n) return 0; return n->isinteger() ? 0 : (float_t *)u; }
inline static int_t   *_to_int(Object *u)   { Number *n = u ? u->cast_number() : 0; if (!n) return 0; return n->isinteger() ? (int_t *)u : 0; }

Object *BinaryEvaluator::evaluate(Object *u, IEvaluator *ev)
{
    Pair *p = PairUtil::cast_pair(u);
    if (!p) return u;

    Object  *q  = ev->evaluate(PairUtil::at(p, 1), ev), *r = ev->evaluate(PairUtil::at(p, 2), ev);

    int_t   *qi = _to_int(q),   *ri = _to_int(r);
    float_t *qf = _to_float(q), *rf = _to_float(r);

    if      (qi && ri) return _int  (evaluate((int)  *qi, (int)  *ri)); // both ints..
    else if (qf && rf) return _float(evaluate((float)*qf, (float)*rf)); // both floats..
    else if (qi && rf) return _float(evaluate((float)*qi, (float)*rf)); // q, int; r, float
    else if (qf && ri) return _float(evaluate((float)*qf, (float)*ri)); // q, float; r, int

    return p;
}