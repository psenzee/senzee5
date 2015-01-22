// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _CONSTPAIR_H
#define _CONSTPAIR_H

#include "streams.h"
#include "read_type.h"

extern const Type type_const_pair;
extern const Type type_const_list;

class ConstPair : public GcObject
{
public:

    int head,
		tail;

    inline ConstPair(int head = 0, int tail = 0) : head(head), tail(tail) {}
    inline ConstPair(IInStream &s) { read(s); }

    void             write(IOutStream &s)       const
    { 
        write_chain_uint(s, head < 0 ? 0 : (unsigned)head);
        write_chain_uint(s, tail < 0 ? 0 : (unsigned)tail);
    }
    void             read(IInStream &s)               { head = read_chain_uint(s); tail = read_chain_uint(s); }
    
    IOutStream      &print(IOutStream &s)       const { write_format(s, "[%d,%d]", head, tail); return s; }
    const Type      &type()                     const { return type_const_pair; }

    ConstPair       *cast_const_pair()                { return this; }
    const ConstPair *cast_const_pair()          const { return this; }
};

class ConstList : public GcObject
{
public:

    std::vector<unsigned> indices;

    inline ConstList() {}
    inline ConstList(IInStream &s) { read(s); }

    void             write(IOutStream &s) const
    {
        write_chain_uint(s, indices.size());
        for (std::vector<unsigned>::const_iterator i = indices.begin(), e = indices.end(); i != e; ++i)
            write_chain_uint(s, *i);
    }
    void             read(IInStream &s)
    { 
        indices.clear();
        unsigned sz = read_chain_uint(s);
        for (unsigned i = 0; i < sz; i++)
            indices.push_back(read_chain_uint(s));
    }

    IOutStream      &print(IOutStream &s)       const
    {
        write_format(s, "[");
        bool first = true;
        for (std::vector<unsigned>::const_iterator i = indices.begin(), e = indices.end(); i != e; ++i)
        {
            if (!first) write_format(s, ",");
            write_format(s, "%d", *i);
            first = false;
        }
        write_format(s, "]");
        return s;
    }
    const Type      &type()                     const { return type_const_list; }

    ConstList       *cast_const_list()                { return this; }
    const ConstList *cast_const_list()          const { return this; }
};

#endif  // _CONSTPAIR_H