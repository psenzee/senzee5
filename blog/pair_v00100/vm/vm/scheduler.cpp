// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "scheduler.h"
#include "return.h"
#include "context.h"
#include "exceptions.h"
#include "gc.h"
#include "vm.h"

#include <string.h>

Scheduler::Scheduler() : count(0)
{
    memset(tasks, 0, sizeof(tasks));
}

void Scheduler::gc()
{
    IMemoryManager *mm = &MemoryManager::manager();
    for (unsigned i = 0; i < count; i++)
        if (tasks[i].context) tasks[i].context->collect(mm);
    mm->clean();
}

void Scheduler::create(Task &task)
{
    for (unsigned i = 0; i < count; i++)
    {
        if (!tasks[i].context)
        {
            tasks[i] = task;
            return;
        }
    }
    tasks[count++] = task;
}

void Scheduler::destroy(Context *context)
{
    if (context)
    {
        for (unsigned i = 0; i < count; i++)
        {
            if (tasks[i].context == context)
            {
                tasks[i].callback.invoke(context->returnvalue());
                tasks[i] = Task();
                delete context;
                return;
            }
        }
    }
}

class Gc : public IGarbageCollector
{
    Scheduler *s;
public:
    Gc(Scheduler *s) : s(s) {}
    void gc() { s->gc(); }
};

bool Scheduler::execute(Context *context)
{
    Object  *r = 0;
    bool     done = false, error = false;
    Gc       collector(this);
    try { done = !Vm::execute(context, &collector); }
    catch (RuntimeException &re)
    {
        fprintf(stderr, "Runtime Exception: %s\r\n", re.what());
        error = true;
    }
    if (done || error)
        destroy(context);
    return done;
}

bool Scheduler::execute()
{
    bool contin = false;
    for (unsigned i = 0; i < count; i++)
    {
        Context *context = tasks[i].context;
        if (context)
            contin |= !execute(context);
    }
    return !contin;
}