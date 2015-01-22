// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "data.h"
#include "codeutil.h"
#include "scriptcompiler.h"
#include "constantpool.h"
#include "pairutil.h"
#include "compileutil.h"
#include "compilervmutil.h"
#include "opcode.h"
#include "compileexception.h"

std::string QuoteCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    if (!ci.compile_return) return "";
    Object *u = PairUtil::at(p, 1);
    // $TODO special case, compiling (quote NIL) or (quote ())
    int id = program->const_index(u, pool);
    return ci.compile_return ? CompilerVmUtil::compile_const(id) : "";
}

std::string LetCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    CompileInfo ci_return(ci); ci_return.compile_return = true;
    std::string c = program->compile(PairUtil::at(p, 2), pool, ci_return);
    if (ci.compile_return) c += Opcode::OC_DUP;
    int ref_index = program->local_index(PairUtil::at(p, 1));
    return c + CompilerVmUtil::compile_set_local(ref_index);
}

std::string SetCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    CompileInfo ci_return(ci); ci_return.compile_return = true;
    std::string c = program->compile(PairUtil::at(p, 2), pool, ci_return);
    if (ci.compile_return) c += Opcode::OC_DUP;
    int local = -1, global = -1;
    Object *symbol = PairUtil::at(p, 1);
    if ((local = program->local_index(symbol, false)) != -1)
        c += CompilerVmUtil::compile_set_local(local);
    else if ((global = program->global_index(symbol, false)) != -1)
        c += CompilerVmUtil::compile_set_global(global);
    else
        throw CompileException(format::format("Undefined variable '%s' in (set ..) expression.", Object::str(symbol).c_str()).c_str());
    return c;
}

std::string DefineGlobalCompilerVm::compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    CompileInfo ci_return(ci); ci_return.compile_return = true;
    std::string c = program->compile(PairUtil::at(p, 2), pool, ci_return);
    if (ci.compile_return) c += Opcode::OC_DUP;
    int ref_index = program->global_index(PairUtil::at(p, 1));
    return c + CompilerVmUtil::compile_set_global(ref_index);
}