// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "compilervmutil.h"

#include "scriptcompiler.h"
#include "constantpool.h"
#include "compileutil.h"
#include "opcode.h"
#include "strings.h"
#include "types.h"

#include "codeutil.h"
#include "constfunction.h"
#include "constpair.h"

#include "compileexception.h"
#include "ielementcompiler.h"

#include <string>
#include <iostream>
#include <sstream>

int CompilerVmUtil::new_local(ScriptCompiler *program)
{
    return program->local_index(Types::create_symbol(m_tmp_symbol()));
    return -1;
}

std::string CompilerVmUtil::compile_set_local(unsigned ref_index)
{
    std::string c;
    if      (ref_index > 65535) { c = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_SETL4), (unsigned)ref_index); }
    else if (ref_index > 255)   { c = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_SETL2), (unsigned short)ref_index); }
    else if (ref_index > 15)    { c = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_SETL1), (unsigned char)ref_index);  }
    else                        { c += (char)((unsigned char)Opcode::OC_SETL_0 + ref_index); }
    return c;
}

std::string CompilerVmUtil::compile_set_global(unsigned ref_index)
{
    std::string c;
    if      (ref_index > 65535) { c = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_SETG4), (unsigned)ref_index); }
    else if (ref_index > 255)   { c = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_SETG2), (unsigned short)ref_index); }
    else                        { c = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_SETG1), (unsigned char)ref_index);  }
    return c;
}

std::string CompilerVmUtil::compile_get_local(unsigned ref_index)
{
    std::string c;
    if      (ref_index > 65535) { c = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_LOCAL4), (unsigned)ref_index); }
    else if (ref_index > 255)   { c = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_LOCAL2), (unsigned short)ref_index); }
    else if (ref_index > 15)    { c = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_LOCAL1), (unsigned char)ref_index); }
    else                        { c += (char)((unsigned char)Opcode::OC_LOCAL_0 + ref_index); }
    return c;
}

std::string CompilerVmUtil::compile_get_global(unsigned ref_index)
{
    std::string c;
    if      (ref_index > 65535) { c = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_GLOBAL4), (unsigned)ref_index); }
    else if (ref_index > 255)   { c = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_GLOBAL2), (unsigned short)ref_index); }
    else                        { c = CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_GLOBAL1), (unsigned char)ref_index); }
    return c;
}

std::string CompilerVmUtil::compile_const(unsigned ref_index)
{
    std::string c;
    if      (ref_index > 65535) { c += CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_CONST ), (unsigned)ref_index); }
    else if (ref_index > 255)   { c += CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_CONST2), (unsigned short)ref_index); }
    else                        { c += CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_CONST1), (unsigned char)ref_index); }
    return c;
}

std::string CompilerVmUtil::compile_func(int ref_index)
{
    std::string c;
    if      (ref_index < -32768 || ref_index > 32767) { c += CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_FUNC ), (unsigned)ref_index); }
    else if (ref_index < -128   || ref_index > 127)   { c += CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_FUNC2), (unsigned short)ref_index); }
    else                                              { c += CodeUtil::append(CodeUtil::code((unsigned char)Opcode::OC_FUNC1), (unsigned char)ref_index); }
    return c;
}

std::string CompilerVmUtil::compile_to_local(ScriptCompiler *program, ConstantPool *pool, Object *u, int ref, const CompileInfo &ci)
{
    std::string c; c += program->compile(u, pool, ci);
    if (ci.compile_return) c += Opcode::OC_DUP;
    c += compile_set_local(ref);
    return c;
}

std::string CompilerVmUtil::compile_local_reference(Object *u, ScriptCompiler *program, const CompileInfo &ci)
{
    int ref = program->local_index(u, false);
    if (ref == -1)
        throw CompileException(format::format("Undefined reference '%s'.", Object::str(u).c_str()).c_str());
    return ci.compile_return ? compile_get_local(ref) : "";
}

std::string CompilerVmUtil::compile_global_reference(Object *u, ScriptCompiler *program, const CompileInfo &ci)
{
    int ref = program->global_index(u, false);
    if (ref == -1)
        throw CompileException(format::format("Undefined reference '%s'.", Object::str(u).c_str()).c_str());
    return ci.compile_return ? compile_get_global(ref) : "";
}

std::string CompilerVmUtil::compile_get_variable(Symbol *u, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci, bool exception_on_error)
{
    if (!ci.compile_return) return "";
    std::string code, name(u ? u->str() : "");
    const char *namecstr = name.c_str();
    int fid = -1;
    if (!u || stricmp(namecstr, "nil") == 0)
        code += (char)Opcode::OC_NIL;
    else if (stricmp(namecstr, "true") == 0)
        code += (char)Opcode::OC_TRUE;
    else if (program->is_defined_local(name))
        code = compile_local_reference(u, program, ci);
    else if (program->is_defined_global(name))
        code = compile_global_reference(u, program, ci);
    else if ((fid = pool->functionid(namecstr, false)) != -1)
        code = compile_func(fid);
    else if (exception_on_error)
        throw CompileException(format::format("Undefined variable '%s'.", namecstr).c_str());
    else
        return "";
    return code;
}

std::string CompilerVmUtil::list_next(ScriptCompiler *program, int listRef, int ref, int jmp)
{
    std::string c;

    c += compile_get_local(listRef);
    c += Opcode::OC_NEXTP;

    std::string end;
    end += compile_get_local(listRef);
    end += Opcode::OC_NEXT;
    end += compile_set_local(ref);

    c += Opcode::OC_JN; CodeUtil::append(c, (short)(jmp + end.size() /** the following instructions */));
    c += end;

    return c;
}

std::string CompilerVmUtil::jump_on_nil(ScriptCompiler *program, int offset)
{
    std::string c;
	c += Opcode::OC_JN; CodeUtil::append(c, (short)offset);
    return c;
}

std::string CompilerVmUtil::jump_on_true(ScriptCompiler *program, int offset)
{
    std::string c;
	c += Opcode::OC_JT; CodeUtil::append(c, (short)offset);
    return c;
}

std::string CompilerVmUtil::m_tmp_symbol()
{
	std::ostringstream oss;
	oss << "@local_" << (m_count++); 
	return oss.str();
}

int CompilerVmUtil::m_count = 0;
