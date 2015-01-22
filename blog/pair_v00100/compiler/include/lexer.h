// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _LEXER_H
#define _LEXER_H

#include "textreader.h"

class Lexer
{
public:

    enum State
    {
        CONST_S_OK,
        CONST_S_INVALID_CHAR_ERROR,
        CONST_S_UNEXPECTED_CHAR_ERROR,
        CONST_S_TOKEN_TOO_LONG_ERROR,
        CONST_S_DELIMITER,
        CONST_S_COMMENT,
        CONST_S_LITERAL,
        CONST_S_IDENTIFIER,
        CONST_S_NUMERIC,
        CONST_S_END
    };

    enum Token
    {
        CONST_T_NONE,
        CONST_T_OPEN_LIST,
        CONST_T_CLOSE_LIST,
        CONST_T_QUOTE,
        CONST_T_META,
        CONST_T_LITERAL,
        CONST_T_IDENTIFIER,
        CONST_T_NUMERIC
    };

    int         line();
    int         column();
    bool        iserror();
    int         errorid();
    const char *errormessage();
    const char *identifier();
	State       state();
    Token       token();

    Lexer(const char *text) : m_reader(text) {}

private:

    TextReader  m_reader;
    std::string m_identifier;
};

#endif // _LEXER_H