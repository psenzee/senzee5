// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _CODE_H
#define _CODE_H

#include "codeutil.h"
#include "strings.h"
#include "functiontable.h"

#include <string>

class ConstantPool;
class FunctionHandle;
class IInStream;
class IOutStream;

class Code
{
public:

    ConstantPool  *pool;
    FunctionTable *functions;

    Code(IInStream &s);
    Code(ConstantPool *pool, const std::string &code);

    void                  write(IOutStream &s) const;
    void                  read(IInStream &s);
    
    FunctionHandle       *function_handle(const char *name);
    FunctionHandle       *function_handle(Symbol *fn);
    std::string          &code(int id);
    void                  set_code(int id, const std::string &code);
    FunctionTable::map_t &function_map();

    static Code          *read(std::string &program);
    static std::string    write(Code *program);
};

#endif  // _CODE_H