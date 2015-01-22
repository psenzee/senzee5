// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _PACKAGE_H
#define _PACKAGE_H

#include <string>
#include <vector>

class Function;

class Export
{
    std::string  m_name;
    Function    *m_function;

public:

    inline Export(const char *name = 0, Function *function = 0) : m_name(!name ? "<?>" : name), m_function(function) {}

    inline const char *name()     { return m_name.c_str(); }
    inline Function   *function() { return m_function; }
};

class Package
{
    enum { MAX_EXPORTS = 1024 };

    std::string       m_name;
    unsigned          m_count;
    Export            m_exports[MAX_EXPORTS];

public:

    inline Package(const char *name) : m_name(!name ? "<?>" : name), m_count(0) {}

    inline const char          *name()                                          { return m_name.c_str(); }
    inline unsigned             count()                                   const { return m_count; }
    inline Export              &export(unsigned i)                              { return m_exports[i]; }
    inline void                 add(const char *name, Function *function)       { m_exports[m_count++] = Export(name, function); }
};

#endif  // _PACKAGE_H 