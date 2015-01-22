// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _PAIR_COMPILER_DATA_H
#define _PAIR_COMPILER_DATA_H

#include "ielementcompiler.h"
#include "arity.h"

class QuoteCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    QuoteCompilerVm() : m_arity(1, 1) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

class LetCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    LetCompilerVm() : m_arity(2, 2) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

class SetCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    SetCompilerVm() : m_arity(2, 2) {}
    
    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

class DefineGlobalCompilerVm : public IElementCompiler
{
    Arity m_arity;
public:

    DefineGlobalCompilerVm() : m_arity(2, 2) {}

    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

#endif  // _PAIR_COMPILER_DATA_H