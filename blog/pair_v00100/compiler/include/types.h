// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _TYPES_H
#define _TYPES_H

#include "object.h"
#include <string>

enum InstrinsicType
{
    TYPEID_OBJECT = 0x1000,
    TYPEID_TYPE,
    TYPEID_PAIR,
    TYPEID_NUMBER,
    TYPEID_STRING,
    TYPEID_SYMBOL,
    TYPEID_MATCH,
    TYPEID_FUNCTION,
    TYPEID_ITERATOR,
    TYPEID_CONSTANT,
    TYPEID_CONTINUATION,
    TYPEID_USEROBJECT,
    TYPEID_CONSTPAIR,
    TYPEID_CONSTLIST,
    TYPEID_CONSTFUNC,

    TYPEID_INTEGER,
    TYPEID_REAL,

    TYPEID_CHAR,
    TYPEID_UCHAR,
    TYPEID_SHORT,
    TYPEID_USHORT,
    TYPEID_INT,
    TYPEID_UINT,
    TYPEID_LONG,
    TYPEID_ULONG,
    TYPEID_FLOAT,
    TYPEID_DOUBLE,
    TYPEID_CHARACTER
};

// intrinsic types
extern const Type type_object;
extern const Type type_type;
extern const Type type_pair;
extern const Type type_number;
extern const Type type_string;
extern const Type type_symbol;
extern const Type type_match;
extern const Type type_function;
extern const Type type_iterator;
extern const Type type_constant;
extern const Type type_continuation;
extern const Type type_userobject;

extern const Type type_const_pair;
extern const Type type_const_list;
extern const Type type_const_function;

extern const Type type_integer;
extern const Type type_real;

extern const Type type_char;
extern const Type type_uchar;
extern const Type type_short;
extern const Type type_ushort;
extern const Type type_int;
extern const Type type_uint;
extern const Type type_long;
extern const Type type_ulong;
extern const Type type_float;
extern const Type type_double;

extern const Type type_character;

// user object types
extern const Type type_raw_pointer;
extern const Type type_stream;
extern const Type type_out_stream;
extern const Type type_in_stream;
extern const Type type_file_out_stream;
extern const Type type_file_in_stream;
extern const Type type_string_out_stream;
extern const Type type_standard_out_stream;

class Symbol;
class String;
class Match;
class Number;
class Character;

class Types
{
public:

    inline static Object *create_true() { return TRUE ? TRUE : m_create_true(); }

    static String        *create_string(const char *s);
    static String        *create_string(const std::string &s);
    static Symbol        *create_symbol(const char *s);
    static Symbol        *create_symbol(const std::string &s);
    static Match         *create_match(const char *s);
    static Match         *create_match(const std::string &s);
    static Number        *create_float(float n);
    static Number        *create_int(int n);
    static Character     *create_char(char c);
    static Object        *create_bool(bool b);
    static Object        *create_user(const Type &type, void *p);
    static Object        *create_raw_pointer(void *p);

    static String        *cast_symbol_or_string(Object *u);
    // if it's a string or symbol, return as is, else create a string representation. if it's null, return null
    static String        *to_string(Object *u);

    static float          to_float(Object *u);
    static unsigned       to_uint(Object *u);
    static const char    *to_cstr(Object *u);
    static void          *to_pointer(Object *u);

private:

    Types();

    static Object *m_create_true();
    static Object *TRUE;
};

#endif  // _TYPES_H