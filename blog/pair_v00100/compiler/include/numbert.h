// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _NUMBERT_H
#define _NUMBERT_H

#include "object.h"
#include "numeric_to_string.h"
#include "streams.h"
#include "types.h"
#include "format.h"

namespace
{
inline const Type &_object_type(char)              { return type_char; }
inline const Type &_object_type(unsigned char)     { return type_uchar; }
inline const Type &_object_type(short)             { return type_short; }
inline const Type &_object_type(unsigned short)    { return type_ushort; }
inline const Type &_object_type(int)               { return type_int; }
inline const Type &_object_type(unsigned int)      { return type_uint; }
inline const Type &_object_type(__int64)           { return type_long; }
inline const Type &_object_type(unsigned __int64)  { return type_ulong; }
inline const Type &_object_type(float)             { return type_float; }
inline const Type &_object_type(double)            { return type_double; }
}

class Number : public GcObject
{
public:
    virtual void   to_string(std::string &s)      const = 0;

    IOutStream    &print(IOutStream &s)           const { std::string sv; to_string(sv); s.write(sv.data(), sv.size()); return s; }

    Number        *cast_number()                        { return this; }
    const Number  *cast_number()                  const { return this; }

    bool           comparable_to(const Object *u) const { return u && type().type_id == u->type().type_id; } // $TODO, right now compare only if they are exactly equal types

    bool           isinteger()                    const { int id = type().type_id; return id != TYPEID_DOUBLE && id != TYPEID_FLOAT; }
    bool           isreal()                       const { int id = type().type_id; return id == TYPEID_DOUBLE || id == TYPEID_FLOAT; }

//  virtual operator char             ()          const = 0;
//  virtual operator unsigned char    ()          const = 0;
//  virtual operator short            ()          const = 0;
//  virtual operator unsigned short   ()          const = 0;
    virtual operator int              ()          const = 0;
//  virtual operator unsigned int     ()          const = 0;
    virtual operator float            ()          const = 0;
//  virtual operator double           ()          const = 0;
    virtual operator __int64          ()          const = 0;
//  virtual operator unsigned __int64 ()          const = 0;
};

template <class T>
class NumberT : public Number
{
    typedef NumberT<T> self;

public:

    T value;

    NumberT(T val) : value(val) {}

    void           set(const T &v)                   { value = v; }
    void           to_string(std::string &s)   const { format::numeric_to_string(value, s); }
    const   Type  &type()                      const { return _object_type(value); }

    int            compare_to(const Object *u) const
    { 
        const Number *n = u->cast_number();
        if (n)
        {
            T nval = (T)*n;
            if      (value < nval) return -1;
            else if (value > nval) return  1;
        }
        return 0;
    }

    inline operator char              ()       const { return (char)value; }
    inline operator unsigned char     ()       const { return (unsigned char)value; }
    inline operator short             ()       const { return (short)value; }
    inline operator unsigned short    ()       const { return (unsigned short)value; }
    inline operator int               ()       const { return (int)value; }
    inline operator unsigned int      ()       const { return (unsigned int)value; }
    inline operator float             ()       const { return (float)value; }
    inline operator double            ()       const { return (double)value; }
    inline operator __int64           ()       const { return (__int64)value; }
    inline operator unsigned __int64  ()       const { return (unsigned __int64)value; }
};

class Character : public NumberT<int>
{
public:
    inline Character(int val) : NumberT<int>(val) {}
    IOutStream &print(IOutStream &s) const { unsigned char c = (unsigned char)value; s.write(&c, 1); return s; }
    const Type &type()               const { return type_character; }
};

#endif  // _NUMBERT_H