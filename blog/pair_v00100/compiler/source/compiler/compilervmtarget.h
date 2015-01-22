// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _COMPILERVMTARGET_H
#define _COMPILERVMTARGET_H

#include "icompiletarget.h"

#include <map>
#include <string>

class CompilerVmTarget : public ICompileTarget
{
    std::map<std::string, IElementCompiler *> m_map;

    inline void m_put(const char *k, IElementCompiler *v) { m_map[std::string(k)] = v; }

public:

    std::map<std::string, IElementCompiler *> &compile_map();

    static CompilerVmTarget *INSTANCE;

private:

    CompilerVmTarget() {}
};

#endif  // _COMPILERVMTARGET_H