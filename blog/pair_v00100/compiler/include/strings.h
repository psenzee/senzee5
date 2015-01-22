// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _STRINGS_H
#define _STRINGS_H

#include "object.h"
#include <string>

#include "streams.h"
#include "types.h"
#include "iterator.h"

class String : public GcObject
{
    std::string m_str;

public:

    inline String() {}
    inline String(const String &s) : m_str(s.str()) {}
    inline String(const std::string &s) : m_str(s) {}

    inline const std::string &str()                             const { return m_str; }
    inline std::string       &str()                                   { return m_str; }
    inline const char        *c_str()                           const { return m_str.c_str(); }

    IOutStream               &print(IOutStream &s)              const;
	Object                   *copy()                            const;

    bool                      comparable_to(const Object *u)    const;
    int                       compare_to(const Object *u)       const;

    Iterator                 *iterator();
    int                       length()                          const;
    Object                   *at(int i);

    String                   *cast_string();
    const String             *cast_string()                     const;
    const Type               &type()                            const;
};

class Symbol : public String
{
public:
    inline Symbol()                                 {}
    inline Symbol(const char *s)        : String(s) {}
	inline Symbol(const std::string &s) : String(s) {}

    String           *cast_string();
    const String     *cast_string() const;
    Symbol           *cast_symbol();
    const Symbol     *cast_symbol() const;
    const Type       &type()        const;
};

class Match : public String
{
public:
    inline Match()                                 {}
    inline Match(const char *s)        : String(s) {}
    inline Match(const std::string &s) : String(s) {}

    String           *cast_string();
    const String     *cast_string() const;
    Match            *cast_match();
    const Match      *cast_match()  const;
    const Type       &type()        const;
};

class StringIterator : public Iterator
{
    String            *m_obj;
    const std::string *m_str;
    size_t             m_iter;
public:
    inline StringIterator(String *obj, const std::string &str) : m_str(&str), m_iter(0), m_obj(obj) {}

    void    apply(ICollectableVisitor *v) const;

    Object *next();
    bool    hasnext() { return m_iter != m_str->size(); }
};

#endif  // _STRINGS_H