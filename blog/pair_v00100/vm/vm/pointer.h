// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _POINTER_H
#define _POINTER_H

#include "context.h"
#include "object.h"
#include "streams.h"
#include "gc.h"

class Pointer : public GcObject
{
public:

    void *pointer;

    Pointer(const Type &type, void *pointer) : m_type(type), pointer(pointer) {}

    const Type         &type()                 const { return m_type; }

    Pointer            *cast_pointer()               { return this; }
    const Pointer      *cast_pointer()         const { return this; }

    void               *data()                       { return pointer; }

    virtual void       *raw_pointer()                { return pointer; }
    virtual const void *raw_pointer()          const { return pointer; }

private:

    const Type &m_type;
};

class StaticPointer : public Pointer
{
public:
    inline StaticPointer(const Type &type, void *pointer) : Pointer(type, pointer) { ICollectable::created--; }
    ~StaticPointer() { ICollectable::created--; }
};

template <typename T>
class StringPointer : public Pointer
{
public:
    StringPointer(const Type &type, void *pointer) : Pointer(type, pointer) {}
    IOutStream &print(IOutStream &s) const
    {
        if (!pointer)
            write_format(s, "[%s/NIL]", type().type_name);
        else write(s, ((T *)pointer)->str());
        return s;
    }
};

#endif  // _POINTER_H

