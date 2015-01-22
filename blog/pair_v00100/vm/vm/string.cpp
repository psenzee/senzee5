// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "string.h"

#include <string>
#include "numbert.h"
#include "types.h"
#include "exceptions.h"

IOutStream &String::print(IOutStream &s) const
{
    s.write(m_str.data(), m_str.size());
    return s;
}

bool String::comparable_to(const Object *u) const
{
    const Type &ut = u->type();
    return ut.of_type(type_string) || ut.of_type(type_symbol);
}

int String::compare_to(const Object *u) const
{
    const Type &ut = u->type();
    if (ut.of_type(type_string) || ut.of_type(type_symbol))
    {
        String *s = (String *)u;
        return strcmp(c_str(), s->c_str());
    }
    return 0;
}

Iterator *String::iterator()
{
    return _gc_new(new StringIterator(this, m_str));
}

unsigned String::length() const
{
    return m_str.size();
}

Object *String::get(unsigned i)
{
    return (size_t)i >= m_str.size() ? 0 : Types::create_char(m_str[i]);
}

String *String::cast_string()
{
    return this;
}

const String *String::cast_string() const
{
    return this;
}

const Type &String::type() const
{
    return type_string;
}

static inline char cast_to_char(Object *u)
{
    Character *c = static_cast<Character *>(cast_to_type(u, type_char, true, false));
    return c ? (char)c->value : 0;
}

void String::set(unsigned i, Object *u)
{
    if (i < m_str.size())
        m_str[i] = cast_to_char(u);
}

Object *String::remove(unsigned i)
{
    throw RuntimeException("Unimplemented.");
    return 0;
}

void String::add(Object *u)
{
    m_str += cast_to_char(u);
}

void String::append(Iterator *u)
{
    throw RuntimeException("Unimplemented.");
}

Object *StringIterator::next()
{
    return Types::create_char((*m_str)[m_iter++]);
}

void StringIterator::apply(ICollectableVisitor *v) const
{
    if (v->visit(this) && m_obj)
        m_obj->apply(v);
}