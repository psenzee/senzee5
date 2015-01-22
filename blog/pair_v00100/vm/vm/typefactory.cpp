// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "types.h"
#include "symbol.h"
#include "string.h"
#include "numbert.h"
#include "pointer.h"
#include "gc.h"
#include "vm.h"

Symbol *Types::create_symbol(const char *s)
{
    return _gc_new(new Symbol(s));
}

Symbol *Types::create_symbol(const std::string &s)
{
    return _gc_new(new Symbol(s));
}

String *Types::create_string(const char *s)
{
    return _gc_new(new String(s));
}

String *Types::create_string(const std::string &s)
{
    return _gc_new(new String(s));
}

Number *Types::create_float(float n)
{
    return _gc_new(new NumberT<float>(n));
}

Number *Types::create_int(int n)
{
    return _gc_new(new NumberT<int>(n));
}

Character *Types::create_char(char c)
{
    return _gc_new(new Character(c)); // $TODO these should be pooled
}

Object *Types::create_bool(bool b)
{
    return b ? Vm::VM_TRUE : 0;
}

Object *Types::create_pointer(const Type &type, void *p)
{
    return _gc_new(new Pointer(type, p));
}

Object *Types::create_pointer(void *p)
{
    return _gc_new(new Pointer(type_pointer, p));
}

String *Types::to_string(Object *u) // if it's a string or symbol, return as is, else create a string representation. if it's null, return null
{
    String *s = 0;
    if (u)
    {
        if ((s = u->cast_string()) != 0) return s;
        if ((s = u->cast_symbol()) != 0) return s;
        s = Types::create_string(Object::str(u));
    }
    return s;
}

float Types::to_float(Object *u)
{
    Number *n = u->cast_number();
    return n ? (float)*n : 0.f;
}

unsigned Types::to_uint(Object *u)
{
    Number *n = u->cast_number();
    return n ? (int)*n : 0;
}

const char *Types::to_cstr(Object *u)
{
    String *s = Types::to_string(u);
    return s ? s->c_str() : 0;
}

void *Types::to_pointer(Object *u)
{
    /*
    Pointer *p = u ? u->cast_pointer() : 0;
    return p ? p->raw_pointer() : 0;
    */
    return u ? u->data() : 0;
}