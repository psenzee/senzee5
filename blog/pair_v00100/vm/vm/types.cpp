// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "types.h"

// intrinsic types
const Type type_object              (TYPEID_OBJECT,       "object"                           );
const Type type_type                (TYPEID_TYPE,         "type",                &type_object);
const Type type_pair                (TYPEID_PAIR,         "pair",                &type_object);
const Type type_number              (TYPEID_NUMBER,       "number",              &type_object);
const Type type_symbol              (TYPEID_SYMBOL,       "symbol",              &type_object);
const Type type_string              (TYPEID_STRING,       "string",              &type_object);
const Type type_function            (TYPEID_FUNCTION,     "function",            &type_object);
const Type type_iterator            (TYPEID_ITERATOR,     "iterator",            &type_object);
const Type type_constant            (TYPEID_CONSTANT,     "constant",            &type_object);
const Type type_import              (TYPEID_IMPORT,       "import",              &type_constant);

const Type type_pointer             (TYPEID_POINTER,      "pointer",             &type_object);
const Type type_array               (TYPEID_ARRAY,        "array",               &type_object);

const Type type_integer             (TYPEID_INTEGER,      "integer",             &type_number);
const Type type_real                (TYPEID_REAL,         "real",                &type_number);

const Type type_int                 (TYPEID_INT,          "int",                 &type_integer);
const Type type_float               (TYPEID_FLOAT,        "float",               &type_real);
const Type type_char                (TYPEID_CHAR,         "char",                &type_integer);

// user object types
const Type type_stream              (TYPEID_MAX + 0,      "stream",              &type_pointer);
const Type type_out_stream          (TYPEID_MAX + 1,      "out-stream",          &type_stream);
const Type type_in_stream           (TYPEID_MAX + 2,      "in-stream",           &type_stream);
const Type type_file_out_stream     (TYPEID_MAX + 3,      "file-out-stream",     &type_out_stream);
const Type type_file_in_stream      (TYPEID_MAX + 4,      "file-in-stream",      &type_in_stream);
const Type type_string_out_stream   (TYPEID_MAX + 5,      "string-out-stream",   &type_out_stream);
const Type type_standard_out_stream (TYPEID_MAX + 6,      "standard-out-stream", &type_out_stream);