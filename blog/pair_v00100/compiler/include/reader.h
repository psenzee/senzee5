// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _READER_H
#define _READER_H

#include <string>
#include <stack>

#include "lexer.h"

/** Reader.<p>
 *
 *  The Reader.  Scans, parses a text
 *  string and constructs object(s) from it.<p>
 *
 *  @author     Paul D Senzee
 *  @version    0.5
 *
 */

class Object;

class Reader
{
public:

    Reader() : m_lexer(0) {}

    Object *read(const char *text);

private:
    Object *m_read();
    Lexer   m_lexer;

};

#endif // _READER_H