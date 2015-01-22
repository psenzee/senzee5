// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _ARITY_H
#define _ARITY_H

class Arity
{
public:
    short min, max;
    inline Arity(int min = 0, int max = 0) : min((short)min), max((short)max) {}
    inline Arity(const Arity &a) : min(a.min), max(a.max)                     {}
};

#endif  // _ARITY_H