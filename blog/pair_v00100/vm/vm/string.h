// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _STRING_H
#define _STRING_H

#include "object.h"
#include <string>

#include "streams.h"
#include "types.h"
#include "iterator.h"
#include "collection.h"

class String : public Collection
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

    String                   *cast_string();
    const String             *cast_string()                     const;
    const Type               &type()                            const;

    void                     *data()                                  { return (void *)m_str.data(); }

    bool                      comparable_to(const Object *u)    const;
    int                       compare_to(const Object *u)       const;

    // collection implementation..
    Iterator                 *iterator();
    unsigned                  length()                          const;
    Object                   *get(unsigned i);
    void                      set(unsigned i, Object *u);
    Object                   *remove(unsigned i);
    void                      add(Object *u);
    void                      append(Iterator *u);
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

#endif  // _STRING_H