// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _DISASSEMBLER_H
#define _DISASSEMBLER_H

#include <string>

#include "object.h"

class Code;
class FunctionHandle;

class Disassembler
{
public:

    static std::string disassemble(Code *program);

private:

    static int           m_disassembleLine(Code *program, const char *code, int index, std::ostream &s);
    static int           m_disassembleArgs(std::ostream &s, Code *program, const char *code, int index, const char *pattern);
    static std::string  &m_appendComment(std::string &s, const char *comment);
    static std::string   m_writeCodeBytes(Code *program, const char *lineHeader);
    static std::string   m_functionHeaderComment(Code *program, FunctionHandle *f);
    static std::string   m_getConstComment(Code *program, int id);
	inline static int    m_int  (const char *code, int index) { return *((int   *)(code + index)); }
	inline static short  m_short(const char *code, int index) { return *((short *)(code + index)); }

	inline static std::string m_type(Object *r)
	{
		std::string s(!r ? "???" : r->type().type_name);
		return s;
	}
};

#endif  // _DISASSEMBLER_H