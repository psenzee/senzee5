// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _IEVALUATOR_H
#define _IEVALUATOR_H

#include <string>

class Object;
class Pair;

class IEvaluator
{
public:
    virtual ~IEvaluator() {}

    virtual Object *evaluate(Object *u, IEvaluator *root) = 0;
};

class IEvaluatorMap
{
public:
    virtual ~IEvaluatorMap() {}

    virtual IEvaluator *get(const char *s)        = 0;
    virtual IEvaluator *get(const std::string &s) = 0;
};

#endif  // _IEVALUATOR_H