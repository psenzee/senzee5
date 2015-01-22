// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

class Context;

#include "return.h"

class Task
{
public:

    Context             *context;
    Return               callback;

    inline Task(Context *context = 0, Return &callback = Return())
        : context(context), callback(callback) {}
};

class Scheduler
{
public:

    Scheduler();

    void gc();

    void create(Task &task);
    void destroy(Context *context);
    bool execute(Context *context);
    bool execute();

private:  

    enum { MAX_TASKS = 256 };

    Task     tasks[MAX_TASKS];
    unsigned count;
};

#endif  // _SCHEDULER_H