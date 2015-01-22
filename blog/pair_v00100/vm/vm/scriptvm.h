// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _SCRIPTVM_H
#define _SCRIPTVM_H

#include "return.h"
#include <string>

class Object;
class Context;
class Code;
class Scheduler;
class ScriptVm;
class Function;

class IInvoker
{
public:
    virtual ~IInvoker() {}
    virtual void call(Object **arguments, unsigned argcount, Return &callback) = 0;
};

typedef Object *(*native_function_t)(Object **arguments, unsigned argcount);

class ScriptVm
{
    Scheduler *m_scheduler;

public:

    ScriptVm();

    Context     *create(const std::string &program);
    void         start(Context *context, Return &callback = Return());
    void         execute();

    void         registerfunction(const char *function, native_function_t func, int min_argument_count = 0, int max_argument_count = -1);

    enum CallConvention { CALL_STDCALL, CALL_CDECL };

    void         registerfunction(const char *function, void *func, CallConvention convention, char return_val, const char *format);

    IInvoker    *function(Context *context, const char *name);
};

#endif  // _SCRIPTVM_H