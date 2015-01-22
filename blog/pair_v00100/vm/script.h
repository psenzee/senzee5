// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _SCRIPT_H
#define _SCRIPT_H

#include <string>

class Code;

class Script
{
public:

	static Code *read(const std::string &mem);
	static Code *read(const char *fname);
    static bool  run(const std::string &code);
    static void  destroy(Code *code);
};

#endif  // _SCRIPT_H
