// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _COMPILERVMUTIL_H
#define _COMPILERVMUTIL_H

#include <string>

class ScriptCompiler;
class ConstantPool;
class Object;
class Symbol;
class CompileInfo;

class CompilerVmUtil
{
public:

    static std::string compile_set_local(unsigned ref_index);
    static std::string compile_set_global(unsigned ref_index);
    static std::string compile_get_local(unsigned ref_index);
    static std::string compile_get_global(unsigned ref_index);
    static std::string compile_const(unsigned ref_index);
    static std::string compile_func(int ref_index);

    static std::string compile_global_reference(Object *u, ScriptCompiler *program, const CompileInfo &ci);
    static std::string compile_local_reference(Object *u, ScriptCompiler *program, const CompileInfo &ci);

    static std::string compile_get_variable(Symbol *u, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci, bool exception_on_error = true);

    static int         new_local(ScriptCompiler *program);
    static std::string compile_to_local(ScriptCompiler *program, ConstantPool *pool, Object *u, int ref, const CompileInfo &ci);

    static std::string list_next(ScriptCompiler *program, int listRef, int ref);
    static std::string list_next(ScriptCompiler *program, int listRef, int ref, int jmp);

    static std::string jump_on_nil(ScriptCompiler *program, int offset);
    static std::string jump_on_true(ScriptCompiler *program, int offset);

private:
    
	static std::string m_tmp_symbol();
    static int m_count;

    CompilerVmUtil();
};

#endif  // _COMPILERVMUTIL_H