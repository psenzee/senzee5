// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "script.h"

#include "vm/pairiterator.h"
#include "vm/frame.h"
#include "vm/functiontable.h"
#include "vm/code.h"
#include "vm/import.h"
#include "vm/opcode.h"
#include "vm/continuation.h"
#include "vm/context.h"
#include "vm/scriptvm.h"
#include "vm/function.h"
#include "vm/exceptions.h"
#include "vm/streams.h"

#include "vm/functions/functions.h"

//#include "vm/symbol.h" // $todo for testing only

#include <string>

Code *Script::read(const std::string &mem)
{
    MemoryInStream s(mem.data(), mem.size());
	return new Code(s);
}

Code *Script::read(const char *fname)
{
    FileInStream f(fname);
	Code *c = new Code(f);
	f.close();
	return c;
}

bool Script::run(const std::string &code)
{
    Object *r = 0;
    ScriptVm svm;

    try
    {
        function::register_common_functions(svm);
        Context *context = svm.create(code);
        svm.start(context);/*
        Context *fnc = new Context(*context);
        IInvoker *invoker = svm.function(fnc, "this-is-my-function");
        Object *args[] = { new Symbol("YoYo"), new Symbol("YoYo2"), new Symbol("YoYo3") };
        invoker->call(args, 3);*/
        svm.execute();
    }
    catch (LinkException &e)
    {
        fprintf(stderr, "Unable to Link Script: %s\r\n", e.what());
        throw e;
    }
    catch (RuntimeException &e)
    {
        fprintf(stderr, "Runtime Error in Script: %s\r\n", e.what());
        throw e;
    }
    catch (std::exception &e)
    {
        fprintf(stderr, "Unexpected Error in Script: %s\r\n", e.what());
        throw e;
    }/*
    catch (...)
    {
        fprintf(stderr, "Internal Error in Script\r\n");
        return false;
    }*/
    if (r)
        printf("\r\n%s\r\n", Object::str(r).c_str());
    return true;
}

void Script::destroy(Code *code)
{
    MemoryManager::manager().clean();
    printf("\n-- GC: objects created %u, destroyed %u\n", ICollectable::created, ICollectable::destroyed);
    if (code) delete code;
}