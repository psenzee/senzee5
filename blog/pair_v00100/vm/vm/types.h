// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _TYPES_H
#define _TYPES_H

#include "object.h"

enum InstrinsicType
{
    TYPEID_OBJECT = 0x10,
    TYPEID_TYPE,
    TYPEID_PAIR,
    TYPEID_NUMBER,
    TYPEID_SYMBOL,
    TYPEID_STRING,
    TYPEID_FUNCTION,
    TYPEID_ITERATOR,
    TYPEID_CONSTANT,
    TYPEID_IMPORT,
    TYPEID_POINTER,
    TYPEID_ARRAY,

    TYPEID_INTEGER,
    TYPEID_REAL,

    TYPEID_INT,
    TYPEID_FLOAT,
    TYPEID_CHAR,

    TYPEID_MAX
};

// intrinsic types
extern const Type type_object;
extern const Type type_type;
extern const Type type_pair;
extern const Type type_number;
extern const Type type_symbol;
extern const Type type_string;
extern const Type type_function;
extern const Type type_iterator;
extern const Type type_constant;
extern const Type type_import;
extern const Type type_pointer;
extern const Type type_array;

extern const Type type_integer;
extern const Type type_real;

extern const Type type_int;
extern const Type type_float;
extern const Type type_char;

// user object types
extern const Type type_stream;
extern const Type type_out_stream;
extern const Type type_in_stream;
extern const Type type_file_out_stream;
extern const Type type_file_in_stream;
extern const Type type_string_out_stream;
extern const Type type_standard_out_stream;

class Symbol;
class String;
class Number;
class Character;

class Types
{
public:

    static Symbol     *create_symbol(const char *s);
    static Symbol     *create_symbol(const std::string &s);
    static String     *create_string(const char *s);
    static String     *create_string(const std::string &s);
    static Number     *create_float(float n);
    static Number     *create_int(int n);
    static Character  *create_char(char c);
    static Object     *create_bool(bool b);
    static Object     *create_pointer(const Type &type, void *p);
    static Object     *create_pointer(void *p);

    // if it's a string or symbol, return as is, else create a string representation. if it's null, return null
    static String     *to_string(Object *u);

    static float       to_float(Object *u);
    static unsigned    to_uint(Object *u);
    static const char *to_cstr(Object *u);
    static void       *to_pointer(Object *u);

private:

    Types();
};

#endif  // _TYPES_H