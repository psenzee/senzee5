// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "context.h"
#include "code.h"
#include "functionhandle.h"
#include "exceptions.h"
#include "functiontable.h"
#include "constantpool.h"

#include "format.h"

#include "function.h"
#include "package.h"
#include "config.h"
#include "pairutil.h"
#include "util.h"

Object **Context::globals = 0;

Object **Context::global(unsigned i)
{
    if (!globals)
    {
        globals = new Object * [MAX_GLOBALS];
        memset(globals, 0, sizeof(Object *) * MAX_GLOBALS);
    }
    return globals + i;
}

Context::Context(Code *program, Package &package) : program(program), local_pool(0), local_pool_at(0)
{
    global(0); // make sure global array is initialized..

    local_pool_at = local_pool = new Object * [MAX_LOCALS];
    fnnative = new Function * [MAX_FUNCTIONS];
    memset(fnnative, 0, sizeof(Function *) * MAX_FUNCTIONS);

    // link..
	for (int i = 0, sz = package.count(); i < sz; i++)
	{
	    Export &exp = package.export(i);
		link(exp.name(), exp.function());
	}

    program->functions->resolve(this);

    Function *main = function(-1);
    call(main, 0);
}

Context::Context(Context &other) : program(other.program), stack(other.stack), frames(other.frames), local_pool(0), local_pool_at(0)
{
    fnnative = new Function * [MAX_FUNCTIONS];
    memcpy(fnnative, other.fnnative, sizeof(Function *) * MAX_FUNCTIONS);
    local_pool = new Object * [MAX_LOCALS];
    size_t pool_size = other.local_pool_at - other.local_pool;
    local_pool_at = local_pool + pool_size;
    memcpy(local_pool, other.local_pool, pool_size * sizeof(Object *));
}

void Context::clear()
{
    stack.clear();
    local_pool_at = local_pool;
    frames.clear();
}

int Context::functionid(const char *name)
{
    return strcmp(name, "MAIN") == 0 ? -1 : program->pool->functionid(name);
}

Context::~Context()
{
    if (local_pool)
        delete [] local_pool;
    local_pool_at = local_pool = 0;
    if (fnnative)
        delete [] fnnative;
    fnnative = 0;
}

void Context::collect(IMemoryManager *mm)
{
    for (unsigned i = 0; i < MAX_GLOBALS; i++)
    {
        Object *p = globals[i];
        if (p) mm->collect(p);
    }
    if (program)
        program->collect(mm);
    for (Object **i = local_pool, **e = local_pool_at; i != e; ++i)
        if (*i) mm->collect(*i);
    for (std::vector<Object *>::iterator i = stack.begin(), e = stack.end(); i != e; ++i)
        mm->collect(*i);
    for (std::vector<Frame>::iterator i = frames.begin(), e = frames.end(); i != e; ++i)
        (*i).collect(mm);
}

std::string Context::currentfunctionname()
{
    int id = frame()->function->id;
    return id == -1 ? "MAIN" : functionname(id);
}

std::string Context::functionname(int id)
{
    if (id == -1) return "MAIN";
    Object *u = program->pool->constant(id);
    return !u ? "<unknown>" : Object::str(u);
}

std::string Context::position()
{  
    return format::format("offset %d in function '%s'", frame()->ip, currentfunctionname().c_str());
}

Function *Context::function(int fnindex)
{
    if (fnindex < -1 || fnindex >= MAX_FUNCTIONS)
        throw LinkException(format::format("Function index #%d is out of range (0-%d).", fnindex, (MAX_FUNCTIONS - 1)).c_str());
    Function *fun = 0;
    if (fnindex < 0)
        fun = program->functions->handle(fnindex);
    else
    {
        fun = fnnative[fnindex];
        if (!fun)
		    fun = program->functions->handle(fnindex);
    }
	if (!fun)
        throw LinkException(format::format("Can't find function '%s' with index #%d", functionname(fnindex).c_str(), fnindex).c_str());
    return fun;
}

Object *Context::constant(int index)
{
    Object *object = program->pool->constant(index);
    if (!object)
        throw LinkException(format::format("Can't find constant #%d", index).c_str());
    return object;
}

void Context::apply(Function *function, Object *list) // $TODO T or F? this won't work with user defined functions??
{
    unsigned count = PairUtil::copy(list, stack);
    Util::reverse(&stack[stack.size() - count], count);
	function->evaluate(this, stack, count);
}

Object **Context::allocatelocals(unsigned count)
{
    Object **locals = local_pool_at;
    local_pool_at += count;
    return locals;
}

void Context::freelocals(unsigned count)
{
    local_pool_at -= count;
}

void Context::doreturn()
{
    Frame *f = frame();
    freelocals(f->localcount);
    frames.pop_back();
}

void Context::link(const char *name, Function *fn)
{
	if (program->pool->isfunction(name))
		fnnative[program->pool->functionid(name)] = fn;
}

void Context::link(Package &package)
{
    // link..
	for (int i = 0, sz = package.count(); i < sz; i++)
	{
	    Export &exp = package.export(i);
		link(exp.name(), exp.function());
	}
}