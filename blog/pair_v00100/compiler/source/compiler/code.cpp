// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "code.h"
#include "constantpool.h"
#include "functionhandle.h"
#include "streams.h"
#include "strings.h"

Code::Code(IInStream &s) : functions(0), pool(0)
{ 
	read(s);
}

Code::Code(ConstantPool *pool, const std::string &code) : functions(new FunctionTable), pool(pool)
{ 
	set_code(-1, code);
}

void Code::write(IOutStream &s) const
{
    pool->write(s);
    if (functions)
        functions->write(s);
}

void Code::read(IInStream &s)
{
	if (pool)
		delete pool;
	if (functions)
		delete functions;
    pool      = new ConstantPool(s);
    functions = new FunctionTable(s);
}

FunctionHandle *Code::function_handle(const char *name)
{
    Symbol s(name);
    return function_handle(&s);
}

FunctionHandle *Code::function_handle(Symbol *fn)
{
    return functions->handle(pool->constantid(fn));
}

std::string &Code::code(int id)
{
    return functions->handle(id)->code;
}

void Code::set_code(int id, const std::string &code)
{
    functions->map()[id] = new FunctionHandle(id, 64, 10, code);
}

FunctionTable::map_t &Code::function_map()
{
	return functions->map();
}