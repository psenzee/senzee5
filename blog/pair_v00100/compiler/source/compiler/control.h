// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _PAIR_COMPILER_CONTROL_H
#define _PAIR_COMPILER_CONTROL_H

#include "ielementcompiler.h"
#include "arity.h"

class ProcedureCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    ProcedureCompilerVm() : m_arity(0, -1) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

class IfCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    IfCompilerVm(int min_arity = 2, int max_arity = 3) : m_arity(min_arity, max_arity) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

class WhileCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    WhileCompilerVm() : m_arity(2, -1) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

class ForEachCompilerVm : public IElementCompiler
{
    Arity m_arity;    
public:

    ForEachCompilerVm() : m_arity(1, -1) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

#endif  // _PAIR_COMPILER_CONTROL_H