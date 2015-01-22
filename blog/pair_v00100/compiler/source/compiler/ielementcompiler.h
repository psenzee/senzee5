// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _IELEMENTCOMPILER_H
#define _IELEMENTCOMPILER_H

#include <string>

class Pair;
class Object;
class ConstantPool;
class ScriptCompiler;
class IEvaluator;
class Arity;

class CompileInfo
{
public:

    bool compile_return,
         compile_final_return,
         compile_yield;

    CompileInfo() : compile_return(true), compile_final_return(true), compile_yield(false) {}
};

inline CompileInfo compile_return(const CompileInfo &ci)
{
    CompileInfo cci(ci);
    cci.compile_return = true;
    return cci;
}

inline CompileInfo compile_no_return(const CompileInfo &ci)
{
    CompileInfo cci(ci);
    cci.compile_return = cci.compile_final_return = false;
    return cci;
}

inline CompileInfo compile_final_return(const CompileInfo &ci)
{
    CompileInfo cci(ci);
    cci.compile_return       = false;
    cci.compile_final_return = true;
    return cci;
}

class IElementCompiler
{
public:

	virtual ~IElementCompiler() {}

    virtual const Arity &arity()                                                                              const = 0;
    virtual std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)       = 0;
};

#endif  // _IELEMENTCOMPILER_H