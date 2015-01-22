// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "scriptvm.h"
#include "vm.h"
#include "symbol.h"
#include "package.h"
#include "opcode.h"
#include "code.h"
#include "context.h"
#include "frame.h"
#include "continuation.h"
#include "exceptions.h"
#include "constantpool.h"
#include "streams.h"
#include "config.h"
#include "ffifunction.h"
#include "iterator.h"
#include "castutil.h"
#include "format.h"
#include "pairutil.h"
#include "util.h"
#include "scheduler.h"

Package &common_package()
{
    static Package *package_ptr = 0, package("common");
    if (!package_ptr)
        package_ptr = &package;
    return *package_ptr;
}

ScriptVm::ScriptVm() : m_scheduler(0)
{
    m_scheduler = new Scheduler;
}

void ScriptVm::start(Context *context, Return &callback)
{
    m_scheduler->create(Task(context, callback));
}

Context *ScriptVm::create(const std::string &program)
{
    return new Context(Code::read(program), common_package());
}

void ScriptVm::execute()
{
    while (!m_scheduler->execute()) {}
}

void ScriptVm::registerfunction(const char *function, native_function_t func, int min_argument_count, int max_argument_count)
{
    // $todo package not currently used, all added to "common"
    common_package().add(function, new FfiFunction(function, func, min_argument_count, max_argument_count));
}

void ScriptVm::registerfunction(const char *function, void *func, CallConvention convention, char out, const char *format)
{
    // $todo package not currently used, all added to "common"
    common_package().add(function, new NativeFunction(function, func, convention == CALL_STDCALL ? NativeCaller::CALL_STDCALL : NativeCaller::CALL_CDECL, out, format));
}

class ScriptInvoker : public IInvoker
{
    ScriptVm          *vm;
    Context           *context;
    Function          *function;

public:

    inline ScriptInvoker(ScriptVm *vm, Context *context, Function *fn) 
        : vm(vm), context(context), function(fn) {}

    void call(Object **args, unsigned argcount, Return &callback)
    {
        context->clear();
        for (int i = (int)argcount - 1; i >= 0; --i)
            context->stack.push_back(args[i]);
        context->call(function, argcount);
        vm->start(context, callback);
    }
};

IInvoker *ScriptVm::function(Context *context, const char *name)
{
    int fid = context->functionid(name);
    return new ScriptInvoker(this, context, context->function(fid));
}