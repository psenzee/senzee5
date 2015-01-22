// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "ffifunction.h"
#include "exceptions.h"
#include "format.h"
#include "streams.h"
#include "util.h"

#include "nativecaller.h"

static IOutStream &_print(IOutStream &s, const std::string &name, const void *p)
{
    write_format(s, "[#native-fn/%s/%p]", name.c_str(), p);
    return s;
}

static void _wrong_arg_count(const Arity &expected, unsigned provided, const std::string &name)
{
    throw RuntimeException(
        format::format("Wrong number of arguments (%u) provided for function '%s'; expected at least %u and no more than %u.\n",
        provided, name.c_str(), expected.min, expected.max).c_str());
}

IOutStream &FfiFunction::print(IOutStream &s) const
{
    return _print(s, m_name, this);
}

void FfiFunction::evaluate(Context *context, std::vector<Object *> &stack, unsigned count)
{
    bool too_few = (int)count < m_arity.min;
    if (too_few || (m_arity.max != -1 && (int)count > m_arity.max))
        _wrong_arg_count(m_arity, count, m_name);
    Object *result = m_function((Object **)((&stack.back() + 1) - count), count);
    stack.resize(stack.size() - (count - 1));
	stack[stack.size() - 1] = result;
}

IOutStream &NativeFunction::print(IOutStream &s) const
{
    return _print(s, m_name, this);
}

void NativeFunction::evaluate(Context *context, std::vector<Object *> &stack, unsigned count)
{
    if (count != m_native.arguments())
        _wrong_arg_count(m_arity, count, m_name);
    Object *result = m_native.call((Object **)((&stack.back() + 1) - count));
    stack.resize(stack.size() - (count - 1));
    stack[stack.size() - 1] = result;
}