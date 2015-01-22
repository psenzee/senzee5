// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <exception>

class RuntimeException : public std::exception
{
public:
	RuntimeException(const std::string &message) : std::exception(message.c_str()) {}
	RuntimeException(const char *message)        : std::exception(message)         {}
};

class LinkException : public std::exception
{
public:
    LinkException(const std::string &message) : std::exception(message.c_str()) {}
    LinkException(const char *message)        : std::exception(message)         {}
};

// for debug:

inline void debug_break()
{
    __asm int 3;
}

#define THROW(X) { debug_break(); throw X; }

#endif  // _EXCEPTIONS_H 