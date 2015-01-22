// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _CONSTFUNCTION_H
#define _CONSTFUNCTION_H

#include <string>

#include "object.h"
#include "streams.h"

extern const Type type_const_function;

class ConstFunction : public GcObject
{
    std::string m_name;

public:

	inline ConstFunction(const std::string &name) : m_name(name) {}
	inline ConstFunction(const char *name)        : m_name(!name ? "" : name) {}

	const std::string   &name()                     const { return m_name; }
    IOutStream          &print(IOutStream &s)       const { s.write(m_name.data(), m_name.size()); return s; }
    const Type          &type()                     const { return type_const_function; }

    ConstFunction       *cast_const_function()            { return this; }
    const ConstFunction *cast_const_function()      const { return this; }
};

#endif  // _CONSTFUNCTION_H