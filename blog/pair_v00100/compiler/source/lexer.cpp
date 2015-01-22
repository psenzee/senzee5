// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "lexer.h"
#include <stdlib.h>

int Lexer::line()
{
    return m_reader.line();
}

int Lexer::column()
{
    return m_reader.column();
}

bool Lexer::iserror()
{
    return false;
}

int Lexer::errorid()
{
    return 0;
}

const char *Lexer::errormessage()
{
    return 0;
}

const char *Lexer::identifier()
{
    return m_identifier.c_str();
}

Lexer::State Lexer::state()
{
    return !m_reader.at() ? CONST_S_END : CONST_S_OK;
}

static char _skip_space(TextReader &r)
{
    char c = 0;
    std::string comment;
    while ((c = r.nextnotspace()) == ';')
    {
        comment.clear();
        r.until(comment, '\n');
    }
    return r.at();
}

Lexer::Token Lexer::token()
{
    m_identifier.clear();
    if (!m_reader.at())
        return CONST_T_IDENTIFIER;
    char c = _skip_space(m_reader);
    switch (c)
    {
    case '(' : m_reader.next();                       return CONST_T_OPEN_LIST;
    case ')' : m_reader.next();                       return CONST_T_CLOSE_LIST;
    case '\'': m_reader.next();                       return CONST_T_QUOTE;
	case '\"': m_reader.string_literal(m_identifier); return CONST_T_LITERAL;
    }
    if (m_reader.numeric_literal(m_identifier))
        return CONST_T_NUMERIC;
    m_reader.until(m_identifier, " \r\n\t\f();");
    return m_reader.at() || !m_identifier.empty() ? CONST_T_IDENTIFIER : CONST_T_NONE;
}
