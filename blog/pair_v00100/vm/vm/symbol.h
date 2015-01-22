// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _SYMBOL_H
#define _SYMBOL_H

#include "string.h"

class Symbol : public String
{
public:
    inline Symbol()                                 {}
    inline Symbol(const char *s)        : String(s) {}
	inline Symbol(const std::string &s) : String(s) {}

    String           *cast_string()       { return 0; }
    const String     *cast_string() const { return 0; }
    Symbol           *cast_symbol()       { return this; }
    const Symbol     *cast_symbol() const { return this; }
    const Type       &type()        const { return type_symbol; }
};

#endif  // _SYMBOL_H