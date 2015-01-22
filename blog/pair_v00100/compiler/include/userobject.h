// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _USEROBJECT_H
#define _USEROBJECT_H

#include "object.h"
#include "streams.h"
#include "gc.h"

class UserObject : public GcObject
{
public:

    void *pointer;

    UserObject(const Type &type, void *pointer) : m_type(type), pointer(pointer) {}

    const Type       &type()                 const { return m_type; }

    UserObject       *cast_userobject()            { return this; }
    const UserObject *cast_userobject()      const { return this; }

    void             *raw_data()                   { return pointer; }
    const void       *raw_data()             const { return pointer; }

private:

    const Type &m_type;
};

class StaticUserObject : public UserObject
{
public:
    inline StaticUserObject(const Type &type, void *pointer) : UserObject(type, pointer) { ICollectable::created--; }
    ~StaticUserObject() { ICollectable::created--; }
};

template <typename T>
class StringUserObject : public UserObject
{
public:
    StringUserObject(const Type &type, void *pointer) : UserObject(type, pointer) {}
    IOutStream &print(IOutStream &s) const
    {
        if (!pointer)
            write_format(s, "[%s/NIL]", type().type_name);
        else write(s, ((T *)pointer)->str());
        return s;
    }
};

#endif  // _USEROBJECT_H 