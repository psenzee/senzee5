// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "object.h"
#include "functiontable.h"
#include "functionhandle.h"
#include "read_type.h"

void FunctionTable::m_read(IInStream &s)
{
    m_functions.clear();
    unsigned count = read_type<unsigned short>(s);
    FunctionHandle *f = 0;
    for (unsigned i = 0; i < count; i++)
    {
        f = _gc_new(new FunctionHandle(s));
        m_functions[f->id] = f;
    }
}

void FunctionTable::write(IOutStream &s) const
{
    write_type<unsigned short>(s, m_functions.size());
    for (map_t::const_iterator i  = m_functions.begin(), end = m_functions.end(); i != end; ++i)
        (*i).second->write(s);
}