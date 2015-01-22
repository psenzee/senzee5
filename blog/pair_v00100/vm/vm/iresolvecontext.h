// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _IRESOLVECONTEXT_H
#define _IRESOLVECONTEXT_H

class Object;
class Function;

class IResolveContext
{
public:
    virtual ~IResolveContext() {}

    virtual Function  *function(int index) = 0;
    virtual Object    *constant(int index) = 0;
};

#endif  /** _IRESOLVECONTEXT_H */