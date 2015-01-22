// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _COMPILEEXCEPTION_H
#define _COMPILEEXCEPTION_H

#include <exception>
#include <string>

class CompileException : public std::exception
{
	std::string m_message;

public:

    CompileException(const std::string &message) : std::exception(message.c_str()), m_message(message) {}
	CompileException(const char *message)        : std::exception(message), m_message(message)         {}

	inline const char *message(void) { return m_message.c_str(); }
};

#endif  // _COMPILEEXCEPTION_H