// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _CODE_H
#define _CODE_H

#include <string>

class IInStream;
class ConstantPool;
class FunctionTable;
class IMemoryManager;

class Code
{
public:

    ConstantPool  *pool;
    FunctionTable *functions;

    Code(IInStream &s);
    Code(const Code &c);

    void                  collect(IMemoryManager *mm);

    void                  read(IInStream &s);
    std::string          &get(int id);
    static Code          *read(const std::string &program);
};

#endif  // _CODE_H