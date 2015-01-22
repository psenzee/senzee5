// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _UNIFY_H
#define _UNIFY_H

#include <map>
#include <string>

class Object;

class Unifier
{
public:

    static Object *unify(Object *pattern, Object *data);
};

#endif  // _UNIFY_H