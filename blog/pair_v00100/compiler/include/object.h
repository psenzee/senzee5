// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _OBJECT_H
#define _OBJECT_H

#include <string>

#include "gc.h"
#include "exceptional.h"

class IOutStream;

class Iterator;
class Pair;
class Number;
class String;
class Symbol;
class Match;
class Function;
class ConstPair;
class ConstList;
class ConstFunction;
class Continuation;
class UserObject;

#define DEFINE_OBJECT_CAST(TYPE, FN_NAME)                                                  \
virtual TYPE             *FN_NAME()                      { return 0; }                     \
virtual const TYPE       *FN_NAME()                const { return 0; }                     \
inline static TYPE       *FN_NAME(Object *e)             { return e ? e->FN_NAME() : 0; }  \
inline static const TYPE *FN_NAME(const Object *e)       { return e ? e->FN_NAME() : 0; }

class Type;

class Object : public ICollectable
{
public:

    virtual ~Object();

    virtual IOutStream                 &print(IOutStream &s)           const; 
    virtual const Type                 &type()                         const;
    virtual Object                     *cast(const Type &type);
    virtual const Object               *cast(const Type &type)         const;

    virtual void                       *raw_data()                           { return  0; }
    virtual const void                 *raw_data()                     const { return  0; }

    virtual bool                        comparable_to(const Object *u) const { return  false; }
    virtual int                         compare_to(const Object *u)    const { return  0; }

    virtual Iterator                   *iterator();
    virtual int                         length()                       const { return -1; }
    virtual Object                     *at(int i)                            { return  0; }

    static std::string                  str(const Object *u);
    static bool                         equal(const Object *a, const Object *b);

    DEFINE_OBJECT_CAST(Type,            cast_type);
    DEFINE_OBJECT_CAST(Pair,            cast_pair);
    DEFINE_OBJECT_CAST(Number,          cast_number);
    DEFINE_OBJECT_CAST(String,          cast_string);
    DEFINE_OBJECT_CAST(Symbol,          cast_symbol);
    DEFINE_OBJECT_CAST(Match,           cast_match);
    DEFINE_OBJECT_CAST(Function,        cast_function);
    DEFINE_OBJECT_CAST(Iterator,        cast_iterator);
    DEFINE_OBJECT_CAST(ConstFunction,   cast_const_function);
    DEFINE_OBJECT_CAST(ConstPair,       cast_const_pair);
    DEFINE_OBJECT_CAST(ConstList,       cast_const_list);
    DEFINE_OBJECT_CAST(Continuation,    cast_continuation);
    DEFINE_OBJECT_CAST(UserObject,      cast_userobject);
};

#undef DEFINE_OBJECT_CAST

typedef Collectable<Object> GcObject;
typedef Tracked<Object>     TrackedObject;
typedef Plain<Object>       PlainObject;

class Type : public PlainObject
{
public:

    const Type *parent;

    unsigned    type_id;
    const char *type_name;

    Type(unsigned type_id, const char *type_name, const Type *parent = 0) : type_id(type_id), type_name(type_name), parent(parent) {}

    Type         *cast_type()                  { return this; }
    const Type   *cast_type()            const { return this; }
    const Type   &type()                 const;

    IOutStream   &print(IOutStream &s)   const;

    virtual bool  of_type(const Type &u) const { return type_id == u.type_id || (parent && parent->of_type(u)); }
};

Object *cast_to_type(Object *object, const Type &type, bool nullable, throw_exception_fn_t exception_handler = 0);
void   *cast_to_data(Object *object, const Type &type, bool nullable, throw_exception_fn_t exception_handler = 0);

#endif  // _OBJECT_H