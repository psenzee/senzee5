// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "singleopcode.h"
#include "codeutil.h"
#include "pairutil.h"
#include "constantpool.h"
#include "compileutil.h"
#include "opcode.h"

std::string SingleOpcodeCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    bool pushes_result = (m_compile_properties & PUSHES_RESULT) || ci.compile_return,
         side_effect   = (m_compile_properties & SIDE_EFFECT) != 0;
    Pair *tp  = Object::cast_pair(PairUtil::tail(p));
    std::string code(CompileUtil::concatenate_compile(tp, pool, program, 
                     (pushes_result || side_effect) ? compile_return(ci) : ci));
    if (ci.compile_return)
        return code + (char)m_opcode;
    if (side_effect)
    {
        code += (char)m_opcode;
        if (pushes_result)
            return code + (char)Opcode::OC_POP;
    }
    return code;
}

std::string SingleOpcodeLengthCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    bool pushes_result = (m_compile_properties & PUSHES_RESULT) || ci.compile_return,
         side_effect   = (m_compile_properties & SIDE_EFFECT) != 0;
    Pair *tp  = Object::cast_pair(PairUtil::tail(p));
    int   len = PairUtil::length(tp);
    std::string code(CompileUtil::concatenate_compile(tp, pool, program, 
                     (pushes_result || side_effect) ? compile_return(ci) : ci)),
                op(CodeUtil::append(CodeUtil::code(m_opcode), (short)len));
    if (ci.compile_return)
        return code + op;
    if (side_effect)
    {
        code += op;
        if (pushes_result)
            return code + (char)Opcode::OC_POP;
    }
    return code;
}