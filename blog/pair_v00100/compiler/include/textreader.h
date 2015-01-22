// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _TEXTREADER_H
#define _TEXTREADER_H

#include <string>

class TextReader
{
public:

    inline TextReader(const char *s = 0) : m_text(s), m_column(0), m_line(0) {}

    inline unsigned line()                          const { return m_line; }
    inline unsigned column()                        const { return m_column; }
    inline char     at()                            const { return *m_text; }
    inline char     next()                                { char c = *m_text; if (*m_text) m_text++; return m_advance(c); }
    char            nextnotspace();
    void            untilspace(std::string &s);
    void            until(std::string &s, char c);
    void            until(std::string &s, const char *c);
    bool            string_literal(std::string &s);
    bool            numeric_literal(std::string &s);

private:

    const char *m_text;
    unsigned    m_column, m_line;

    enum { TAB_SPACES = 4 };

    inline char m_advance(char c)
    {
        switch (c)
        {
        case '\n': m_column = 0; m_line++; break;
        case '\r':                         break; // ignore return..
        case '\t': m_column += TAB_SPACES; break;
        default  : m_column++;             break;
        }
        return c;
    }
    void m_advance(const char *i, const char *e);
};

#endif // _TEXTREADER_H