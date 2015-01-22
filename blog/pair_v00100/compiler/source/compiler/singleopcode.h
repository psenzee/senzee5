// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _PAIR_COMPILER_SINGLEOPCODE_H
#define _PAIR_COMPILER_SINGLEOPCODE_H

#include "ielementcompiler.h"
#include "arity.h"

enum CompileProperty { NO_SIDE_EFFECT = 0, SIDE_EFFECT = 1, PUSHES_RESULT = 2 };

class SingleOpcodeCompilerVm : public IElementCompiler
{
    Arity           m_arity;
    unsigned char   m_opcode;    
    unsigned        m_compile_properties;

public:

    SingleOpcodeCompilerVm(unsigned char opcode, unsigned compile_properties, int arity_min, int arity_max) : m_compile_properties(compile_properties), m_opcode(opcode), m_arity(arity_min, arity_max) {}
    SingleOpcodeCompilerVm(unsigned char opcode, unsigned compile_properties, const Arity &arity) : m_compile_properties(compile_properties), m_opcode(opcode), m_arity(arity) {}

    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

class SingleOpcodeLengthCompilerVm : public IElementCompiler
{
    Arity           m_arity;
    unsigned char   m_opcode;    
    unsigned        m_compile_properties;

public:

    SingleOpcodeLengthCompilerVm(unsigned char opcode, unsigned compile_properties, int arity_min, int arity_max) : m_compile_properties(compile_properties), m_opcode(opcode), m_arity(arity_min, arity_max) {}
    SingleOpcodeLengthCompilerVm(unsigned char opcode, unsigned compile_properties, const Arity &arity) : m_compile_properties(compile_properties), m_opcode(opcode), m_arity(arity) {}

    std::string  compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    const Arity &arity() const { return m_arity; }
};

#endif  // _PAIR_COMPILER_SINGLEOPCODE_H