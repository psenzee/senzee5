// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "constantpool.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include "codeutil.h"
#include "numbert.h"
#include "strings.h"
#include "streams.h"
#include "numbert.h"
#include "pairutil.h"
#include "format.h"
#include "read_type.h"
#include "types.h"

#include "constpair.h"
#include "constfunction.h"

#include "constantpool.h"

void ConstantPool::write(IOutStream &s) const
{
    int count = m_list.size();
	write_type<int>(s, count);
    for (int i = 0; i < count; i++)
        m_write(s, m_list[i]);
}

ConstFunction *ConstantPool::create_function(const char *name)
{
    return _gc_new(new ConstFunction(name));
}

ConstantPool::ConstantPool()
{
    constantid(0, true); // make sure that nil is the first item
}

ConstantPool::ConstantPool(IInStream &s)
{
    read(s);
}

ConstantPool::ConstantPool(const ConstantPool &other) : m_list(other.m_list), m_index(other.m_index)
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
    if (!u) return u;
    ConstPair *p = u->cast_const_pair();
    if (p) return m_list[index] = m_pair(p);
    ConstList *list = u->cast_const_list();
    if (list) return m_pair(list);
    return u;
}

bool ConstantPool::issymbol(const char *name)
{
    Symbol symbol(name);
    return m_index.find(m_idstr(&symbol)) != 0;
}

bool ConstantPool::isfunction(const char *name)
{
    ConstFunction fn(name);
    return m_index.find(m_idstr(&fn)) != 0;
}

int ConstantPool::symbolid(const char *name, bool create_if_undefined)
{
    Symbol symbol(name);
    return constantid(&symbol, create_if_undefined);
}

int ConstantPool::functionid(const char *name, bool create_if_undefined)
{
    ConstFunction fn(name);
    return constantid(&fn, create_if_undefined);
}

int ConstantPool::constantid(Object *u, bool create_if_undefined)
{
    int index = -1;
    if (!u)
    {
        if (create_if_undefined && m_list.empty())
        {
            m_list.push_back(0);
            m_index[m_idstr(u)] = 0;
        }
        return 0;
    }
    else
    {
        std::string s;
        if (!u->cast_pair())
            s = m_idstr(u);
        else
        {
            Pair *p = u->cast_pair();
            if (PairUtil::length(p) <= 3)
            {
                // collapse a pair
                Object *h = PairUtil::head(u), *t = PairUtil::tail(u);
                int head = !h ? 0 : constantid(h),
                    tail = !t ? 0 : constantid(t);
                ConstPair *cp = _gc_new(new ConstPair(head, tail));
                s = format::format("pair#%d,%d", head, tail);
                u = cp;
            }
            else
            {
                ConstList *list = _gc_new(new ConstList);
                PairIter i(p), e(0);
                for (; i != e; ++i)
                    list->indices.push_back(constantid(*i));
                s = "list#" + Object::str(list);
                u = list;
            }
        }

        if (m_index.find(s) != m_index.end())
            index = m_index[s];
        else
        {
            if (!create_if_undefined)
                return -1;
            m_index[s] = index = m_list.size();
            m_list.push_back(u);
        }
    }
    return index;
}

void ConstantPool::read(IInStream &s)
{
    m_list.clear();
    m_index.clear();
    int count = read_type<int>(s);
    for (int i = 0; i < count; i++)
        constantid(m_read(s));
}

std::string ConstantPool::m_idstr(Object *u)
{
    return std::string(m_typestr(m_type(u))) + "#" + Object::str(u);
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
    case SYMBOL:    return Types::create_symbol(read_prefix_str<char,  char>(s));
    case FUNCTION:  return _gc_new(new ConstFunction(read_prefix_str<char, char>(s)));
    case PAIR:      return _gc_new(new ConstPair(s));
    case LIST:      return _gc_new(new ConstList(s));
//  case LONG:      return Types::create_long(read_type<__int64>(s));
//  case DOUBLE:    return Types::create_double(read_type<double>(s));
    case NIL:       return 0;
    }
    return 0;
}

void ConstantPool::m_write(IOutStream &s, Object *u) const
{
    int type = m_type(u);
    write_type<char>(s, type);
    String            *us  = Types::cast_symbol_or_string(u);
    Number            *un  = Object::cast_number(u);
    ConstFunction     *uf  = Object::cast_const_function(u);
    const std::string *uss = !us ? (!uf ? 0 : &uf->name()) : &us->str();
    switch (type)
    {
    case INT:       write_type<int>(s, (int)*un); break;
    case FLOAT:     write_type<float>(s, (float)*un); break;
    case STRING:    write_prefix_str<short, char>(s, *uss); break;
    case SYMBOL:    
    case FUNCTION:  write_prefix_str<char,  char>(s, *uss); break;
    case PAIR:      ((ConstPair *)u)->write(s); break;
    case LIST:      ((ConstList *)u)->write(s); break;
//  case LONG:      write_type<__int64>(s, (__int64)*un); break;
//  case DOUBLE:    write_type<double>(s, (double)*un); break;
    case NIL:       break;
    }
}

const char *ConstantPool::m_typestr(int type)
{
    switch (type)
    {
    case INT:       return "INT";
    case FLOAT:     return "FLOAT";
    case LONG:      return "LONG";
    case DOUBLE:    return "DOUBLE";
    case STRING:    return "STRING";
    case SYMBOL:    return "SYMBOL";
    case FUNCTION:  return "FUNCTION";
    case PAIR:      return "PAIR";
    case LIST:      return "LIST";
    case NIL:       return "NIL";
    default:        return "<XX>";
    }
}

int ConstantPool::m_type(Object *u)
{
    if (u)
    {
        switch (u->type().type_id)
        {
        case TYPEID_STRING:          return STRING;
        case TYPEID_SYMBOL:          return SYMBOL;
        case TYPEID_LONG:
        case TYPEID_ULONG:           return LONG;
        case TYPEID_FLOAT:           return FLOAT;
        case TYPEID_DOUBLE:          return DOUBLE;
        case TYPEID_CHAR:
        case TYPEID_UCHAR:
        case TYPEID_SHORT:
        case TYPEID_USHORT:
        case TYPEID_INT:
        case TYPEID_UINT:            return INT;
        case TYPEID_CONSTPAIR:       return PAIR;
        case TYPEID_CONSTLIST:       return LIST;
        case TYPEID_CONSTFUNC:       return FUNCTION;
        }
    }
    return NIL;
}

Pair *ConstantPool::m_pair(ConstPair *p)
{
    return !p ? 0 : PairUtil::pair(constant(p->head), constant(p->tail));
}

Pair *ConstantPool::m_pair(ConstList *list)
{
    if (!list) return 0;
    PairList plist;
    for (std::vector<unsigned>::const_iterator i = list->indices.begin(), e = list->indices.end(); i != e; ++i)
        plist.add(constant(*i));
    return plist.list;
}