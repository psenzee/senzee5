// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _FUNCTIONTABLE_H
#define _FUNCTIONTABLE_H

class IInStream;
class FunctionHandle;
class IResolveContext;

class FunctionTable
{
    FunctionHandle **m_functions;
    void m_read(IInStream &s);
public:
    enum { MAX_FUNCTIONS = 1024 };
    FunctionTable(IInStream &s) : m_functions(0) { m_read(s); }
    FunctionTable(const FunctionTable &other);
    ~FunctionTable();

    void                    collect(IMemoryManager *mm);

    inline FunctionHandle  *handle(int id) { return m_functions[id + 1]; }
    void                    resolve(IResolveContext *context);
};

#endif  /** _FUNCTIONTABLE_H */