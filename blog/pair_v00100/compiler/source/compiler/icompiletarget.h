// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _ICOMPILETARGET_H
#define _ICOMPILETARGET_H

#include <map>
#include <string>

class IElementCompiler;

typedef std::map<std::string, IElementCompiler *> element_compile_map_t;

class ICompileTarget
{
public:

	virtual ~ICompileTarget() {}

	virtual element_compile_map_t &compile_map() = 0;
};

#endif  // _ICOMPILETARGET_H