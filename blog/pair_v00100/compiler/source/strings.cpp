// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "strings.h"

#include <string>
#include "numbert.h"
#include "types.h"

IOutStream &String::print(IOutStream &s) const
{
    s.write(m_str.data(), m_str.size());
    return s;
}

bool String::comparable_to(const Object *u) const
{
    const Type &ut = u->type();
    return ut.of_type(type_string) || ut.of_type(type_symbol) || ut.of_type(type_match);
}

int String::compare_to(const Object *u) const
{
    const Type &ut = u->type();
    if (ut.of_type(type_string) || ut.of_type(type_symbol) || ut.of_type(type_match))
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

int String::length() const
{
    return m_str.size();
}

Object *String::at(int i)
{
    return i < 0 || (size_t)i >= m_str.size() ? 0 : Types::create_char(m_str[i]);
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

String *Match::cast_string()
{ 
    return 0;
}

const String *Match::cast_string() const
{
    return 0;
}

Match *Match::cast_match() 
{ 
    return this;
}

const Match *Match::cast_match() const
{
    return this;
}

const Type &Match::type() const
{
    return type_match;
}

String *Symbol::cast_string()
{ 
    return 0;
}

const String *Symbol::cast_string() const
{
    return 0;
}

Symbol *Symbol::cast_symbol() 
{ 
    return this;
}

const Symbol *Symbol::cast_symbol() const
{
    return this;
}

const Type &Symbol::type() const
{
    return type_symbol;
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