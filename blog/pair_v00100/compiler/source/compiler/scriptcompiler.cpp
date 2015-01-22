// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "scriptcompiler.h"
#include "object.h"
#include "pairutil.h"
#include "icompiletarget.h"
#include "ielementcompiler.h"
#include "constantpool.h"
#include "code.h"
#include "opcode.h"
#include "functionhandle.h"
#include "compilervmutil.h"
#include "constfunction.h"

#include "compileexception.h"
#include "staticevaluator.h"

ScriptCompiler::ScriptCompiler(Pair *root, bool allow_undef, ICompileTarget *target) : m_root(root), m_allow_undefined(allow_undef), m_target(target)
{
    m_compilers = target->compile_map();
    push_local_scope();
}

std::string ScriptCompiler::compile(Object *u, ConstantPool *pool, const CompileInfo &ci)
{
    Pair *p = PairUtil::cast_pair(u);
    return p ? m_compile_pair(p, pool, ci) : m_compile_object(u, pool, ci);
}

int ScriptCompiler::func_index(Object *u, ConstantPool *pool, bool create_new)
{
    return pool->constantid(ConstantPool::create_function(Object::str(u).c_str()), create_new);
}

int ScriptCompiler::const_index(Object *u, ConstantPool *pool, bool create_new)
{
    return pool->constantid(u, create_new);
}

int ScriptCompiler::local_index(Object *u, bool create_new)
{
    if (!u)
        return -1;
	std::string s = Object::str(u);
	int id = -1;
	if (locals().find(s) != locals().end())
		id = locals()[s];
    else if (create_new)
        locals()[s] = (id = locals().size());
    return id;
}

int ScriptCompiler::global_index(Object *u, bool create_new)
{
    if (!u)
        return -1;
    std::string s = Object::str(u);
    int id = -1;
    if (globals().find(s) != globals().end())
        id = globals()[s];
    else if (create_new)
        globals()[s] = (id = globals().size());
    return id;
}

void ScriptCompiler::push_local_scope()
{
    local_map_t m;
    m_locals.push_back(m);
}

void ScriptCompiler::pop_local_scope()
{ 
    m_locals.pop_back();
}

local_map_t &ScriptCompiler::locals()
{ 
    return m_locals.back();
}

global_map_t &ScriptCompiler::globals()
{ 
    return m_globals;
}

void ScriptCompiler::add_function(int id, int localsize, int stacksize, const std::string &code)
{
    add_function(new FunctionHandle(id, localsize, stacksize, code));
}

void ScriptCompiler::add_function(FunctionHandle *fh)
{
    m_functions.push_back(fh);
}

Code *ScriptCompiler::compile_code()
{
	ConstantPool   *pool = 0;
    Code           *c    = 0;
    FunctionHandle *f    = 0;

	try
	{
        pool = new ConstantPool;

        m_root = PairUtil::cast_pair(StaticEvaluator::transform(m_root));

        FileOutStream xformed("xformed.script");
        PairUtil::print(xformed, m_root);
        xformed.close();

        CompileInfo ci;
        std::string code = m_compile_pair(m_root, pool, ci);
	    code += Opcode::OC_END;

        c = new Code(pool, code);
        for (int i = 0, sz = m_functions.size(); i < sz; i++)
		{
            f = m_functions[i];
            c->function_map()[f->id] = f;
		}
	}
	catch (CompileException &ce)
	{
		std::cerr << ce.message() << std::endl;
		if (c) delete c;
		c = 0;
	}

    return c;
}

std::string ScriptCompiler::m_compile_pair(Pair *p, ConstantPool *pool, const CompileInfo &ci)
{
    Object           *op = p->head;
    IElementCompiler *ec = 0;
    
    if (PairUtil::cast_pair(op))
    {
        // scheme style evaluation of function expression
        ec = m_compilers[std::string("_evaluate_invoke")];       
    }
    else
    {
 	    std::string ops(Object::str(op));
        ec = m_compilers[ops];
        int arg_count = PairUtil::length(PairUtil::cast_pair(p->tail));
        if (!ec) ec = m_compilers[std::string("_invoke")];
	    m_arity(ec, arg_count, ops.c_str(), false);
    }
    return ec->compile(p, pool, this, ci);
}

bool ScriptCompiler::is_defined_local(const std::string &name) const
{
    const local_map_t &map = m_locals.back();
    local_map_t::const_iterator i = map.find(name);
    return i != map.end();
}

bool ScriptCompiler::is_defined_global(const std::string &name) const
{
    const global_map_t &map = m_globals;
    global_map_t::const_iterator i = map.find(name);
    return i != map.end();
}

std::string ScriptCompiler::m_compile_object(Object *u, ConstantPool *pool, const CompileInfo &ci)
{
	std::string c;
    int op = 0;
    Symbol *sym = Object::cast_symbol(u);
    if (!u)
        op = Opcode::OC_NIL;
    else if (Object::cast_string(u) && !sym)
        return m_compile_string_literal(u, pool, ci);
    else if (Object::cast_number(u))
        return m_compile_numeric_literal(u, pool, ci);
    else if (sym)
        return CompilerVmUtil::compile_get_variable(sym, pool, this, ci);
    return c += op;
}

std::string ScriptCompiler::m_compile_string_literal(Object *s, ConstantPool *pool, const CompileInfo &ci)
{
    return ci.compile_return ? CompilerVmUtil::compile_const(const_index(s, pool)) : "";
}

std::string ScriptCompiler::m_compile_numeric_literal(Object *n, ConstantPool *pool, const CompileInfo &ci)
{
    if (!ci.compile_return) return "";
    Number *num = Object::cast_number(n);
    NumberT<int> *integer = num && num->isinteger() ? (NumberT<int> *)num : 0;
    std::string c;
    if (integer)
    {
        int v = (int)*integer;
        if      (v < -32768 || v > 32767)   { c += Opcode::OC_INT4; CodeUtil::append(c, v); }
        else if (v < -128   || v > 127)     { c += Opcode::OC_INT2; CodeUtil::append(c, (unsigned short)v); }
        else                                { c += Opcode::OC_INT1; CodeUtil::append(c, (unsigned char)v); }
    }
    else
    {
	    c += CompilerVmUtil::compile_const(const_index(n, pool));
    }
    return c;
}

void ScriptCompiler::m_arity(IElementCompiler *ec, int argCount, const char *fnname, bool function)
{
	std::string message;
	if (ec->arity().min != -1 && argCount < ec->arity().min)
	    message = m_format("Too few arguments in call to $f", fnname, function);
	else if (ec->arity().max != -1 && argCount > ec->arity().max)
	    message = m_format("Too many arguments in call to $f", fnname, function);
	if (message.size() > 0)
		throw CompileException(message.c_str());
}

std::string ScriptCompiler::m_format(const char *format, const char *fnname, bool function)
{
	static std::string fn("function"), op("operator");
	std::string formalname = (function ? fn : op) + " '" + std::string(fnname) + "'",
		        s;
	char ch = 0;
	while (ch = *format++)
	{
		switch (ch)
		{
		case '$':

			ch = *format;
			if      (ch == 'f') { s += formalname; format++; }
			else if (ch == '$')	{ s += '$';        format++; }
			else                  s += ch;
			break;

		default:

			s += ch;
			break;
		}
	}
	return s;
}
