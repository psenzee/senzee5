// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "constantpool.h"

#include <map>

#include "code_t.h"
#include "symbol.h"
#include "numbert.h"
#include "pairutil.h"
#include "format.h"
#include "read_type.h"
#include "import.h"
#include "types.h"

ConstantPool::ConstantPool(IInStream &s)
{
    read(s);
}

ConstantPool::ConstantPool(const ConstantPool &other) : m_list(other.m_list), m_functions(other.m_functions)
{
}

int ConstantPool::count()
{
    return m_list.size();
}

void ConstantPool::collect(IMemoryManager *mm)
{
    for (std::vector<Object *>::iterator i = m_list.begin(), e = m_list.end(); i != e; ++i)
        if (*i) mm->collect(*i);
}

Object *ConstantPool::constant(int index)
{
    if (index == -1 || (unsigned int)index >= m_list.size())
        return 0;
    Object *u = m_list[index];
    return u;
}

bool ConstantPool::isfunction(const char *name)
{
    return m_functions.find(name) != 0;
}

int ConstantPool::functionid(const char *name)
{
    SimpleMap::Entry *e = m_functions.find(name);
    return e ? e->index : -1;
}

void ConstantPool::read(IInStream &s)
{
    m_list.clear();
    m_functions.clear();
    int count = read_type<int>(s);
    for (int i = 0; i < count; i++)
    {
        Object *u = m_read(s);
        if (u && u->type().of_type(type_import))
            m_functions.set(Object::str(u), m_list.size());
        m_list.push_back(u);
    }
}

// read..
Object *ConstantPool::m_read(IInStream &s)
{
    int type = read_type<char>(s);
    switch (type)
    {
    case INT:       return Types::create_int(read_type<int>(s));
    case FLOAT:     return Types::create_float(read_type<float>(s));
    case STRING:    return Types::create_string(read_prefix_str<short, char>(s));
    case SYMBOL:    return Types::create_symbol(read_prefix_str<char, char>(s));
    case FUNCTION:  return _gc_new(new Import(read_prefix_str<char, char>(s)));
    case PAIR:      return m_read_pair(s);
    case LIST:      return m_read_list(s);
    case NIL:       return 0;
    }
    return 0;
}

Pair *ConstantPool::m_read_pair(IInStream &s)
{
    unsigned head = read_chain_uint(s), tail = read_chain_uint(s);
    return PairUtil::pair(constant((int)head), constant((int)tail));
}

Pair *ConstantPool::m_read_list(IInStream &s)
{
    PairList list;
    unsigned count = read_chain_uint(s);
    for (unsigned i = 0; i < count; i++)
        list.add(constant(read_chain_uint(s)));
    return list.list;
}