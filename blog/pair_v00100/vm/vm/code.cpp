// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "code.h"
#include "functionhandle.h"
#include "functiontable.h"
#include "constantpool.h"
#include "streams.h"
#include "config.h"

Code::Code(IInStream &s) : functions(0), pool(0)
{ 
	read(s);
}

Code::Code(const Code &c) : functions(new FunctionTable(*c.functions)), pool(new ConstantPool(*c.pool))
{ 
}

void Code::collect(IMemoryManager *mm)
{
    if (functions)
        functions->collect(mm);
    if (pool)
        pool->collect(mm);
}

void Code::read(IInStream &s)
{
	if (pool)
        delete pool;
    pool = new ConstantPool(s);
	if (functions)
        delete functions;
    functions = new FunctionTable(s);
}

std::string &Code::get(int id)
{
    return functions->handle(id)->code;
}

Code *Code::read(const std::string &program)
{
    MemoryInStream s(program.data(), program.size());
    return new Code(s);
}