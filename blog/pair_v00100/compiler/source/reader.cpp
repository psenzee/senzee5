// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "reader.h"
#include "pairutil.h"
#include "strings.h"
#include "numbert.h"

Object *Reader::read(const char *text)
{
	m_lexer = Lexer(text);
    Object *e = m_read();
	return e;
}

static bool end(Lexer &lexer)
{
    return lexer.state() == Lexer::CONST_S_END || lexer.iserror();
}

Object *read_list(Lexer &lexer);

Object *create_identifier(const char *s)
{
    if      (s[0] == '?')             return Types::create_match(s);
    else if (stricmp(s, "nil")  == 0) return 0;
    else if (stricmp(s, "true") == 0) return Types::create_true();
    return Types::create_symbol(s);
}

Object *read_next(Lexer &lexer, Lexer::Token token)
{
    const char *s = lexer.identifier();
    switch (token)
    {
    case Lexer::CONST_T_OPEN_LIST:  return read_list(lexer);
    case Lexer::CONST_T_QUOTE:      return PairUtil::pair(Types::create_symbol("quote"), PairUtil::pair(read_next(lexer, lexer.token()), 0));
    case Lexer::CONST_T_LITERAL:    return Types::create_string(s);
    case Lexer::CONST_T_IDENTIFIER: return create_identifier(s);
    case Lexer::CONST_T_NUMERIC:	return (strchr(s, '.')) ? Types::create_float((float)atof(s)) : Types::create_int(atoi(s));
    }
    return 0;
}

Object *read_list(Lexer &lexer)
{
    PairList list;
    while (!end(lexer))
    {
        Lexer::Token token = lexer.token();
        if (token == Lexer::CONST_T_CLOSE_LIST)
            return list.list;
		if (token == Lexer::CONST_T_IDENTIFIER && strcmp(lexer.identifier(), ".") == 0)
        {
            token = lexer.token();
            list.tail->tail = read_next(lexer, token);
            token = lexer.token();
            if (token != Lexer::CONST_T_CLOSE_LIST)
            {
                // throw an exception about a malformed dotted pair!
                printf("Malformed dotted pair!!\n");
            }
            return list.list;
        }
        list.add(read_next(lexer, token));
    }
    return list.list;
}

Object *Reader::m_read()
{
    if (end(m_lexer))
        return 0;
    Lexer::Token t = m_lexer.token();
    return (t == Lexer::CONST_T_OPEN_LIST) ? read_list(m_lexer) : read_next(m_lexer, t);
}
