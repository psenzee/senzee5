// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _PAIR_COMPILER_FUNCTIONS_H
#define _PAIR_COMPILER_FUNCTIONS_H

#include "ielementcompiler.h"
#include "arity.h"

class Pair;
class ConstantPool;
class ScriptCompiler;
class FunctionHandle;

class CallCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    CallCompilerVm() : m_arity(1, -1) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

class InvokeCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    InvokeCompilerVm() : m_arity(0, -1) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

class ClosureCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    ClosureCompilerVm() : m_arity(0, -1) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

class InvokeExprCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    InvokeExprCompilerVm() : m_arity(0, -1) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

class FunctionInstanceCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    FunctionInstanceCompilerVm() : m_arity(1, 1) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

class DefineFunctionCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    DefineFunctionCompilerVm() : m_arity(0, -1) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }

    static FunctionHandle *compile_function_handle(Pair *p, int id, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
};

#endif  // _PAIR_COMPILER_FUNCTIONS_H