// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "object.h"

#include <stdlib.h>
#include <typeinfo>

#include "streams.h"
#include "format.h"
#include "types.h"
#include "iterator.h"

Object::~Object() {}

IOutStream &Object::print(IOutStream &s) const
{
    write_format(s, "[%s/%p]", type().type_name, this);
    return s;
}

Iterator *Object::iterator()
{
    return &NullIterator::instance;
}

std::string Object::str(const Object *u)
{
    if (!u) return "NIL"; 
    StringOutStream os;
    u->print(os);
    return os.str();
}

bool Object::equal(const Object *a, const Object *b)
{
    if ( a ==  b)             return true;
    if (!a || !b)             return false;
    if (!a->comparable_to(b)) return false;
    return a->compare_to(b) == 0;
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

Object *cast_to_type(Object *object, const Type &type, bool nullable, throw_exception_fn_t exception_handler)
{
    if (nullable && !object) return 0;
    if (!object || !object->cast(type))
    {
        if (!exception_handler) return 0;
        throw_exception(exception_handler, "expected instance of type %s; not found.\n", type.type_name);
    }
    return object;
}

void *cast_to_data(Object *object, const Type &type, bool nullable, throw_exception_fn_t exception_handler)
{
    object = cast_to_type(object, type, nullable, exception_handler);
    return !object ? 0 : object->raw_data();
}