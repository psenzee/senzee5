// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "control.h"
#include "codeutil.h"
#include "scriptcompiler.h"
#include "constantpool.h"
#include "pairutil.h"
#include "compileutil.h"
#include "opcode.h"
#include "compilervmutil.h"

std::string ProcedureCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    return CompileUtil::compile_block(PairUtil::cast_pair(p->tail), pool, program, ci);
}

std::string compile_ifxyz(const std::string &test, Object *expr_a, Object *expr_b, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    std::string nop, code; nop += (ci.compile_return ? Opcode::OC_NIL : Opcode::OC_NOP);
    std::string cond_a = !expr_a ? nop : program->compile(expr_a, pool, ci),
                cond_b = !expr_b ? nop : program->compile(expr_b, pool, ci);       
    code += test;
    code += Opcode::OC_JN;
    CodeUtil::append(code, (short)(cond_a.size() + Opcode::width(Opcode::OC_JN)));
    code += cond_a;
    code += Opcode::OC_JMP;
    CodeUtil::append(code, (short)cond_b.size());
    code += cond_b;
    return code;
}

std::string compile_ifxy(const std::string &test, Object *expr, unsigned char conditional_jump_opcode, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    std::string code, expression(program->compile(expr, pool, ci));
    code += test;
    code += (char)conditional_jump_opcode;
    CodeUtil::append(code, (short)expression.size());
    return code + expression;
}

inline std::string compile_ifxy0(const std::string &test, Object *expr, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    return compile_ifxy(test, expr, Opcode::OC_JN, pool, program, ci);
}

inline std::string compile_ifx0z(const std::string &test, Object *expr, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    return compile_ifxy(test, expr, Opcode::OC_JT, pool, program, ci);
}

std::string IfCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    Object *t = PairUtil::at(p, 1), *a = PairUtil::at(p, 2), *b = PairUtil::at(p, 3);
    std::string test = program->compile(t, pool, compile_return(ci));
    if      (ci.compile_return) return compile_ifxyz(test, a, b, pool, program, ci);
    else if (!b)                return compile_ifxy0(test, a,    pool, program, ci);
    else if (!a)                return compile_ifx0z(test, b,    pool, program, ci);
    else                        return compile_ifxyz(test, a, b, pool, program, ci);
}

std::string WhileCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    int end_sz = ci.compile_yield ? 4 : 3;
    std::string test     = program->compile(PairUtil::at(p, 1), pool, compile_return(ci)),
                body     = CompileUtil::compile_block(PairUtil::nthpair(p, 2), pool, program, ci),
                testjump = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_JN),   (short)(body.size() + end_sz));
    short       jmp_sz   = (short)-(int)(test.size() + body.size() + testjump.size() + end_sz);    
    std::string end      = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_JMP), jmp_sz);
    if (ci.compile_yield)
        end = CodeUtil::code((unsigned char)Opcode::OC_YIELD) + end;
    return test + testjump + body + end;
}

std::string ForEachCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    int          listRef  = CompilerVmUtil::new_local(program);
    Pair        *args     = PairUtil::cast_pair(PairUtil::at(p, 1));
    std::string  listCode;

    listCode += CompilerVmUtil::compile_to_local(program, pool, PairUtil::at(args, 1), listRef, compile_return(ci));

    listCode += CompilerVmUtil::compile_get_local(listRef);
    listCode += (unsigned char)Opcode::OC_ITER;

    listCode += CompilerVmUtil::compile_set_local(listRef);

    int         ref      = program->local_index(Object::cast_symbol(args->head)),
                endSize  = ci.compile_yield ? 4 : 3;
    std::string body     = CompileUtil::compile_block(PairUtil::nthpair(p, 2), pool, program, ci),
                begin    = CompilerVmUtil::list_next(program, listRef, ref, body.size() + endSize),
                end;
    if (ci.compile_yield)
        end += (unsigned char)Opcode::OC_YIELD;
    end += (unsigned char)Opcode::OC_JMP;
    CodeUtil::append(end, (short)-(int)(begin.size() + body.size() + endSize));
    return listCode + begin + body + end;
}