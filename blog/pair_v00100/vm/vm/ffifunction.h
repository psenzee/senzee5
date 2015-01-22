// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _FFIFUNCTION_H
#define _FFIFUNCTION_H

#include "function.h"
#include "scriptvm.h"
#include "nativecaller.h"

class FfiFunction : public Function
{
    native_function_t  m_function;
    Arity              m_arity;
    std::string        m_name;

public:

    FfiFunction(const char *name, native_function_t func, int min_argument_count, int max_argument_count) : 
        m_name(name ? name : "<anon>"), m_arity(min_argument_count, max_argument_count), m_function(func) {}

	void          evaluate(Context *context, std::vector<Object *> &stack, unsigned count);
    IOutStream   &print(IOutStream &s) const;

    void         *data()                           { return (void *)m_function; }

    const Arity  &arity()                    const { return m_arity; }
    const char   *name()                     const { return m_name.c_str(); }
};

class NativeFunction : public Function
{
    std::string        m_name;
    NativeCaller       m_native;
    Arity              m_arity;

public:

    NativeFunction(const char *name, void *func, NativeCaller::CallConvention convention, char return_val, const char *format) : 
      m_name(name ? name : "<anon>"), m_native(func, convention, return_val, format) { m_arity = Arity(m_native.arguments(), m_native.arguments()); }

    void          evaluate(Context *context, std::vector<Object *> &stack, unsigned count);
    IOutStream   &print(IOutStream &s) const;

    void         *data()                           { return m_native.data(); }

    const Arity  &arity()                    const { return m_arity; }
    const char   *name()                     const { return m_name.c_str(); }
};

#endif  // _FFIFUNCTION_H