// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "object.h"
#include <vector>
#include "types.h"

class Context;

class Arity
{
public:
    short min, max;
    inline Arity(int min = 0, int max = 0) : min((short)min), max((short)max) {}
    inline Arity(const Arity &a) : min(a.min), max(a.max)                     {}
};

class Function : public PlainObject
{
public:
	virtual ~Function() {}
    virtual const     Arity &arity()                                                                  const = 0;
    virtual void             evaluate(Context *context, std::vector<Object *> &stack, unsigned count)       = 0;

    Function        *cast_function()       { return this; }
    const Function  *cast_function() const { return this; }
    const Type      &type()          const { return type_function; }
};

#endif  // _FUNCTION_H 