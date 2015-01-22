// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "types.h"
#include "strings.h"
#include "numbert.h"
#include "userobject.h"
#include "gc.h"

// intrinsic types
const Type type_object              (TYPEID_OBJECT,         "object"                           );
const Type type_type                (TYPEID_TYPE,           "type",                &type_object);
const Type type_pair                (TYPEID_PAIR,           "pair",                &type_object);
const Type type_number              (TYPEID_NUMBER,         "number",              &type_object);
const Type type_symbol              (TYPEID_SYMBOL,         "symbol",              &type_object);
const Type type_match               (TYPEID_MATCH,          "match",               &type_object);
const Type type_string              (TYPEID_STRING,         "string",              &type_object);
const Type type_function            (TYPEID_FUNCTION,       "function",            &type_object);
const Type type_iterator            (TYPEID_ITERATOR,       "iterator",            &type_object);
const Type type_constant            (TYPEID_CONSTANT,       "constant",            &type_object);
const Type type_continuation        (TYPEID_CONTINUATION,   "continuation",        &type_object);

const Type type_userobject          (TYPEID_USEROBJECT,     "userobject",          &type_object);

const Type type_const_pair          (TYPEID_CONSTPAIR,      "const-pair",          &type_constant);
const Type type_const_list          (TYPEID_CONSTLIST,      "const-list",          &type_constant);
const Type type_const_function      (TYPEID_CONSTFUNC,      "const-function",      &type_constant);

const Type type_integer             (TYPEID_INTEGER,        "integer",             &type_number);
const Type type_real                (TYPEID_REAL,           "real",                &type_number);

const Type type_int                 (TYPEID_INT,            "int",                 &type_integer);
const Type type_long                (TYPEID_LONG,           "long",                &type_integer);
const Type type_float               (TYPEID_FLOAT,          "float",               &type_real);
const Type type_double              (TYPEID_DOUBLE,         "double",              &type_real);
const Type type_character           (TYPEID_CHARACTER,      "character",           &type_integer);

// user object types
const Type type_raw_pointer         (0x293846,              "raw-pointer",         &type_userobject);
const Type type_stream              (0x293847,              "stream",              &type_userobject);
const Type type_out_stream          (0x293848,              "out-stream",          &type_stream);
const Type type_in_stream           (0x293849,              "in-stream",           &type_stream);
const Type type_file_out_stream     (0x29384a,              "file-out-stream",     &type_out_stream);
const Type type_file_in_stream      (0x29384b,              "file-in-stream",      &type_in_stream);
const Type type_string_out_stream   (0x29384c,              "string-out-stream",   &type_out_stream);
const Type type_standard_out_stream (0x29384d,              "standard-out-stream", &type_out_stream);

class True : public Symbol
{
public:
    inline  True() : Symbol("true") { ICollectable::created--;   }
    inline ~True()                  { ICollectable::destroyed--; }
};

Object *Types::TRUE = 0;

Object *Types::m_create_true()
{
    if (!TRUE) TRUE = new True;
    return TRUE;
}

String *Types::create_string(const char *s)
{
    return _gc_new(new String(s));
}

String *Types::create_string(const std::string &s)
{
    return _gc_new(new String(s));
}

Match *Types::create_match(const char *s)
{
    return _gc_new(new Match(s));
}

Match *Types::create_match(const std::string &s)
{
    return _gc_new(new Match(s));
}

Symbol *Types::create_symbol(const char *s)
{
    return _gc_new(new Symbol(s));
}

Symbol *Types::create_symbol(const std::string &s)
{
    return _gc_new(new Symbol(s));
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
    return b ? create_true() : 0;
}

Object *Types::create_user(const Type &type, void *p)
{
    return _gc_new(new UserObject(type, p));
}

Object *Types::create_raw_pointer(void *p)
{
    return _gc_new(new UserObject(type_raw_pointer, p));
}

String *Types::cast_symbol_or_string(Object *u)
{
    String *s = 0;
    if (!u) return 0;
    s = u->cast_string();
    if (!s) s = u->cast_symbol();
    return s;
}

String *Types::to_string(Object *u) // if it's a string or symbol, return as is, else create a string representation. if it's null, return null
{
    String *s = 0;
    if (u && !(s = cast_symbol_or_string(u)))
        s = Types::create_string(Object::str(u));
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
    return u ? u->raw_data() : 0;
}