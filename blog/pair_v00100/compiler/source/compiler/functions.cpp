// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "functions.h"
#include "scriptcompiler.h"
#include "compilervmutil.h"
#include "constantpool.h"
#include "pairutil.h"
#include "compileutil.h"
#include "opcode.h"
#include "functionhandle.h"
#include "constfunction.h"

std::string CallCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    Object *u = PairUtil::at(p, 1);
    p = PairUtil::cast_pair(p->tail);
    int len = PairUtil::length(p) - 1;
    std::string func = CompileUtil::concatenate_compile(PairUtil::cast_pair(p->tail), pool, program, compile_return(ci)) +  // args
                       program->compile(u, pool, compile_return(ci)); // fn reference..
    func += Opcode::OC_CALL;
    func += (unsigned char)len;
    if (!ci.compile_return)
        func += Opcode::OC_POP;
    return func;
}

std::string compile_function(Object *function, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    if (!ci.compile_return) return "";
    std::string code(CompilerVmUtil::compile_get_variable(function->cast_symbol(), pool, program, ci, false));
    if (code.empty()) // temporary, until we get (declare ..) in
        code = CompilerVmUtil::compile_func(program->func_index(function, pool));
    return code;
}

std::string InvokeCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
	int len = PairUtil::length(p) - 1;
	std::string args = CompileUtil::concatenate_compile(PairUtil::cast_pair(p->tail), pool, program, compile_return(ci));
	Object *u = p->head;
	return args + compile_function(u, pool, program, compile_return(ci)) + CodeUtil::code(Opcode::OC_CALL, (unsigned char)len);
}

std::string FunctionInstanceCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    return ci.compile_return ? CompilerVmUtil::compile_func(program->func_index(PairUtil::head(p->tail), pool)) : "";
}

std::string InvokeExprCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    CallCompilerVm callc;
    Pair *pair = PairUtil::pair(0, p);
    return callc.compile(pair, pool, program, ci);
}

std::string DefineFunctionCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    Symbol *fnname = Object::cast_symbol(PairUtil::at(p, 1));
    std::string name(Object::str(fnname));
    int id = program->const_index(pool->create_function(name.c_str()), pool);
    FunctionHandle *fh = compile_function_handle(PairUtil::nthpair(p, 2), id, pool, program, ci);
    program->add_function(fh);
    if (ci.compile_return)
    {
        FunctionInstanceCompilerVm finstc;
        Pair *pair = PairUtil::list(2, 0, fnname);
        return finstc.compile(pair, pool, program, ci);
    }
    return "";
}

FunctionHandle *DefineFunctionCompilerVm::compile_function_handle(Pair *p, int id, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    program->push_local_scope();
    PairIter it(PairUtil::cast_pair(p->head)), e(0);
    for (; it != e; ++it)
        program->local_index(*it);
    std::string body = CompileUtil::compile_block(PairUtil::nthpair(p, 1), pool, program, compile_return(ci));
    int localcount = program->locals().size();
    program->pop_local_scope();
    FunctionHandle *fh =
        new FunctionHandle(id, localcount, 64,
                CodeUtil::append(CodeUtil::code(Opcode::OC_PROC), (short)localcount) + 
                body + CodeUtil::code(Opcode::OC_RET));
    return fh;
}