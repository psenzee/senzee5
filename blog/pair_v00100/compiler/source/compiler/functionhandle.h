// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _FUNCTIONHANDLE_H
#define _FUNCTIONHANDLE_H

#include "object.h"
#include "arity.h"

class IInStream;
class IOutStream;

class FunctionHandle : public GcObject
{
public:

    short          id;
    unsigned short stacksize,
                   localsize;
    std::string    code;
    Arity          arity;

    inline FunctionHandle(IInStream &s) : arity(0, -1) { read(s); }
    inline FunctionHandle(short id, unsigned short localsize, unsigned short stacksize, const std::string &code) :
	                       arity(0, -1), id(id), stacksize(stacksize), localsize(localsize), code(code) {}

    void read(IInStream &s);
    void write(IOutStream &s) const;
};

#endif  // _FUNCTIONHANDLE_H