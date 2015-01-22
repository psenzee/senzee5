// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _VM_H
#define _VM_H

class Object;
class Context;

class IGarbageCollector
{
public:
    virtual ~IGarbageCollector() {}
    virtual void gc() = 0;
};

class Vm
{
    static unsigned INSTRUCTION_COUNT;

    Vm();

public:

    static Object *VM_TRUE;

    static bool execute(Context *context, IGarbageCollector *collector);
};

#endif  // _VM_H