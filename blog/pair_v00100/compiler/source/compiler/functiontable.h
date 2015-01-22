// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _FUNCTIONTABLE_H
#define _FUNCTIONTABLE_H

class IInStream;
class IOutStream;
class FunctionHandle;

#include <map>

class FunctionTable
{
public:

    typedef std::map<int, FunctionHandle *> map_t;

    FunctionTable() {}
    FunctionTable(IInStream &s) { m_read(s); }
    FunctionTable(const FunctionTable &other) : m_functions(m_functions) {}

    void                    write(IOutStream &s) const;
    void                    collect(IMemoryManager *mm);
    inline FunctionHandle  *handle(int id) { return m_functions[id]; }
    inline map_t           &map()          { return m_functions; }

private:

    map_t m_functions;

    void m_read(IInStream &s);
};

#endif  // _FUNCTIONTABLE_H