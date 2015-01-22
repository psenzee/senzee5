// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "textreader.h"
#include <stdlib.h>
#include <ctype.h>

void TextReader::m_advance(const char *i, const char *e)
{
    for (; i != e; ++i) m_advance(*i);
}

char TextReader::nextnotspace()
{ 
    while (*m_text &&  isspace(*m_text))
        next();
    return *m_text;
}

void TextReader::untilspace(std::string &s)
{ 
    while (*m_text && !isspace(*m_text))
        s += next();
}

void TextReader::until(std::string &s, char c)
{ 
    while (*m_text && *m_text != c)
        s += next();
}

void TextReader::until(std::string &s, const char *c) // not efficient, but oh well
{ 
    while (*m_text && !strchr(c, *m_text))
        s += next();
}

bool TextReader::string_literal(std::string &s)
{
    s.clear();
    const char *p = m_text;
    if (*p != '\"')
        return false;
    p++;
    while (*p && *p != '\"')
    {
        char c = *p++;
        if (c == '\n' || c == '\r')
        {
            s.clear();
            throw std::exception("end of line found in string literal");
            return false; // in case we ax the exception
        }
        if (c == '\\')
        {
            c = *p;
            if (!c) break;
            p++;
            switch (c)
            {
            case '\\': s += '\\'; break;
            case 'n' : s += '\n'; break;
            case 'r' : s += '\r'; break;
            case 'f' : s += '\f'; break;
            default:   s += '\\'; s += c; break;
            }
        }
		else
		{
			s += c;
		}
    }
    if (!*p)
    {
        s.clear();
        throw std::exception("end of file found in string literal");
        return false; // in case we ax the exception
    }
    m_advance(m_text, p + 1);
    m_text = ++p;
    return true;
}

bool _parse_hex(const char **p, unsigned *v)
{
    const char *q = *p;
    if (q[0] != '0' || q[1] != 'x')
        return false;
    q += 2;
    const char *t = q;
    *v = strtol(q, const_cast<char **>(&t), 16); 
    if (t != q) { *p = t; return true; }
    return false;
}

bool TextReader::numeric_literal(std::string &s)
{
    s.clear();
    const char *p = m_text;
    bool digit = false, hasdigits = false; // must have at least one digit
    unsigned hexval = 0;
    if (*p == '-' || *p == '+')
        s += *p++;
    else if (*p == '0' && _parse_hex(&p, &hexval))
    {
        m_advance(m_text, p);
        m_text = p;
        char buffer[16];
        sprintf(buffer, "%u", hexval);
        s = buffer;
        return true;
    }
    while (*p && ((digit = isdigit(*p) ? true : false) || *p == '.'))
    {
        hasdigits |= digit;
        s += *p++;
    }
    if (hasdigits)
    {
        m_advance(m_text, p);
        m_text = p;
        return true;
    }
    s.clear();
    return false;
}