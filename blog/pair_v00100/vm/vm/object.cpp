// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "object.h"

#include <stdlib.h>
#include <typeinfo>

#include "streams.h"
#include "exceptions.h"
#include "format.h"
#include "types.h"
#include "iterator.h"
#include "pointer.h"

Object::~Object() {}

IOutStream &Object::print(IOutStream &s) const
{
    write_format(s, "[%s/%p]", type().type_name, this);
    return s;
}

std::string Object::str(const Object *u)
{
    if (!u) return "NIL"; 
    StringOutStream os;
    u->print(os);
    return os.str();
}

Object *Object::cast(const Type &u)
{
    if (this && type().of_type(u))
        return this;
    return 0;
}

const Object *Object::cast(const Type &u) const
{
    if (this && type().of_type(u))
        return this;
    return 0;
}

const Type &Object::type() const
{
    return type_object;
}

const Type &Type::type() const
{
    return type_type;
}

IOutStream &Type::print(IOutStream &s) const
{
    write_format(s, "[#type/%s/%p]", type_name, this);
    return s;
}

static void _error_expected_type(const Type &type)
{
    throw RuntimeException(format::format("Expected instance of type %s; not found.\n", type.type_name).c_str());
}

Object *cast_to_type(Object *object, const Type &type, bool required, bool nullable)
{
    if (nullable && !object) return 0;
    if (!object || !object->cast(type))
    {
        if (required)
            _error_expected_type(type);
        return 0;
    }
    return object;
}

void *cast_to_raw_pointer(Object *object, const Type &type, bool required, bool nullable)
{
    if (nullable && !object) return 0;
    Pointer *p = object->cast_pointer();
    if (!p)
    {
        if (required)
            _error_expected_type(type_pointer);
        return 0;
    }
    p = static_cast<Pointer *>(cast_to_type(object, type, required, nullable));
    return !p ? 0 : p->raw_pointer();
}