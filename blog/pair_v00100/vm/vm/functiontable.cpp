// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "object.h"
#include "functiontable.h"
#include "functionhandle.h"
#include "iresolvecontext.h"
#include "read_type.h"

FunctionTable::FunctionTable(const FunctionTable &other)
{
    m_functions = new FunctionHandle * [MAX_FUNCTIONS];
    for (unsigned i = 0; i < MAX_FUNCTIONS; i++)
        m_functions[i] = other.m_functions[i] ? new FunctionHandle(*other.m_functions[i]) : 0;
}

FunctionTable::~FunctionTable()
{
    if (m_functions)
        delete [] m_functions;
}

void FunctionTable::m_read(IInStream &s)
{
    if (m_functions)
        delete [] m_functions;
    m_functions = new FunctionHandle * [MAX_FUNCTIONS];
    memset(m_functions, 0, MAX_FUNCTIONS * sizeof(FunctionHandle *));
    unsigned count = read_type<unsigned short>(s);
    FunctionHandle *f = 0;
    for (unsigned i = 0; i < count; i++)
    {
        f = _gc_new(new FunctionHandle(s));
        m_functions[f->id + 1] = f;
    }
}

void FunctionTable::collect(IMemoryManager *mm)
{ // $todo - arg, horrifically slow..
    for (unsigned i = 0; i < MAX_FUNCTIONS; i++)
        if (m_functions[i])
            mm->collect(m_functions[i]);
}

void FunctionTable::resolve(IResolveContext *context)
{
    FunctionHandle *fh = 0;
    for (unsigned i = 0; i < MAX_FUNCTIONS; i++)
        if (m_functions[i])
            m_functions[i]->resolve(context);
}
