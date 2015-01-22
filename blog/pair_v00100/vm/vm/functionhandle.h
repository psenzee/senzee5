// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _FUNCTIONHANDLE_H
#define _FUNCTIONHANDLE_H

#include "function.h"
#include "frame.h"
#include "code.h"

class Context;
class IResolveContext;

class FunctionHandle : public Function
{
    Arity                 m_arity;
    std::vector<Object *> m_constants;

public:

    short          id;
    unsigned short stacksize,
                   localsize;
    std::string    code;

    inline FunctionHandle(IInStream &s) : m_arity(0, -1) { read(s); }
    inline FunctionHandle(short id, unsigned short localsize, unsigned short stacksize, const std::string &code) :
	                       m_arity(0, -1), id(id), stacksize(stacksize), localsize(localsize), code(code) {}
    inline FunctionHandle(const FunctionHandle &other) :
                           m_arity(other.m_arity), id(other.id), stacksize(other.stacksize), localsize(other.localsize), code(other.code), m_constants(other.m_constants) {}

    void          apply(ICollectableVisitor *v) const;

    void         *data()                     { return (void *)code.data(); }

    const Arity  &arity() const { return m_arity; }
    void          evaluate(Context *context, std::vector<Object *> &stack, unsigned count);
    void          read(IInStream &s);

    void          resolve(IResolveContext *context);
};

#endif  // _FUNCTIONHANDLE_H 