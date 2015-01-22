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
inline const Type &_object_type(char)  { return type_char; }
inline const Type &_object_type(int)   { return type_int; }
inline const Type &_object_type(float) { return type_float; }
}

class Number : public GcObject
{
public:
    virtual void   to_string(std::string &s)      const = 0;

    IOutStream    &print(IOutStream &s)           const { std::string sv; to_string(sv); s.write(sv.data(), sv.size()); return s; }

    Number        *cast_number()                        { return this; }
    const Number  *cast_number()                  const { return this; }

    bool           comparable_to(const Object *u) const { return u && type().type_id == u->type().type_id; } // $TODO, right now compare only if they are exactly equal types

    bool           isinteger()                    const { int id = type().type_id; return id != TYPEID_FLOAT; }
    bool           isreal()                       const { int id = type().type_id; return id == TYPEID_FLOAT; }

    virtual operator char             ()          const = 0;
    virtual operator int              ()          const = 0;
    virtual operator float            ()          const = 0;
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

    void          *data()                            { return (void *)&value; }

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
    inline operator int               ()       const { return (int)value; }
    inline operator float             ()       const { return (float)value; }
};

class Character : public NumberT<int>
{
public:
    inline Character(int val) : NumberT<int>(val) {}
    IOutStream &print(IOutStream &s) const { unsigned char c = (unsigned char)value; s.write(&c, 1); return s; }
    const Type &type()               const { return type_char; }

//  void       *data()                     { return (void *)&value; }               // little endian, same as above
//  void       *data()                     { return (void *)((char *)&value + 3); } // big endian
};

#endif  // _NUMBERT_H