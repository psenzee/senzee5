// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "disassembler.h"

#include "compiler/opcode.h"
#include "compiler/functionhandle.h"
#include "compiler/functiontable.h"
#include "compiler/code.h"
#include "compiler/constantpool.h"
#include "compiler/constfunction.h"

#include "object.h"

#include <iostream>
#include <sstream>
#include <math.h>

static std::string bytes_to_string(const unsigned char *bytes, unsigned start, unsigned end, const char *header)
{
    std::ostringstream s;
    static const char *hex = "0123456789abcdef";
    bytes += start;
    for (unsigned i = 0, sz = end - start; i < sz; i++)
    {
        if (i % 16 == 0 && header)
            s << '\n' << header;
        s << ' ' << hex[(bytes[i] & 0x0f0) >> 4] 
        << hex[ bytes[i] & 0x00f];
    }
    if (header)
        s << '\n';
    return s.str();
}

static std::string bytes_to_string(const std::string &bytes, const char *header)
{
    return bytes_to_string((const unsigned char *)bytes.data(), 0, (unsigned)bytes.size(), header);
}

enum End { LEFT, RIGHT, BOTH, CENTER };

static std::string justify(const char *cs, size_t fieldsz, End justify = LEFT, bool trunc = false)
{
    size_t csz = strlen(cs);
    csz     = trunc && fieldsz < csz  ? fieldsz : csz;
    fieldsz = !trunc && csz > fieldsz ? csz : fieldsz;
    std::string s(cs, csz);
    if (s.size() >= fieldsz)
        return s;
    static std::string sp(" ");
    while (s.size() < fieldsz)
    {
        switch (justify)
        {
        default:
        case LEFT:   s += sp;         break;
        case RIGHT:  s = sp + s;      break;
        case CENTER: s = sp + s + sp; break;
        }
    }
    return s.size() > fieldsz ? s.substr(0, fieldsz) : s;
}

static std::ostream &append(std::ostream &s, int value, unsigned int fieldsz)
{
    unsigned int sz = value == 0 ? 0 : (int)log10((double)value);
    s << value;
    while (sz < fieldsz) { s << ' '; sz++; }
    return s;
}

static std::string _escape(const std::string &s)
{
    std::string out;
    for (std::string::const_iterator i = s.begin(), e = s.end(); i != e; ++i)
    {
        char c = *i;
        switch (c)
        {
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        case '\f': out += "\\f"; break;
        default:   out += c;     break;
        }
    }
    return out;
}

void bytes(ConstantPool *pool, std::string &s)
{
    unsigned char *memory = new unsigned char [64 * 1024]; // reserve 64k
    MemoryOutStream mos(memory, 64 * 1024);
    pool->write(mos);
    s.assign((char *)memory, mos.size());
    delete [] memory;
}

std::string Disassembler::disassemble(Code *program)
{
    std::string code,
	            pool;
    
    bytes(program->pool, pool);

	std::ostringstream s;

    s << "; bytecode" << std::endl << std::endl << "; constants --" << bytes_to_string(pool, ";")
		<< std::endl;

	s << "; constants (data) -- " << std::endl;

	for (int i = 0, sz = program->pool->count(); i < sz; i++)
	{
		s << "; ";
		append(s, i, 6);
		Object *constant = program->pool->constant(i);
		s << justify(("[" + m_type(constant) + "]").c_str(), 30);
		s << '\'' << _escape(Object::str(constant)) << '\'' << std::endl;
	}

	s << std::endl << "; code" << std::endl
	  << m_writeCodeBytes(program, ";") << std::endl;

    FunctionTable::map_t::iterator  it   = program->function_map().begin(),
		                            end  = program->function_map().end();
    FunctionHandle                 *f    = 0;
	for (; it != end; ++it)
    {
        f = (*it).second;
        s << "\n; " << m_functionHeaderComment(program, f) << '\n';
		code = program->code(f->id);
        int i = 0, sz = code.size();
        while (i < sz)
            i = m_disassembleLine(program, (const char *)code.data(), i, s);
    }

    return s.str();
}

int Disassembler::m_disassembleLine(Code *program, const char *code, int index, std::ostream &s)
{
    char oc = 0;

    append(s, index, 5);

	s << ' ' <<
        justify(("[" + bytes_to_string((const unsigned char *)code, index, index + Opcode::width(code[index]), 0) + "]").c_str(),
			30, LEFT, false) << ' ';

    s << Opcode::name(oc = code[index++]);

    switch (oc)
    {
    case Opcode::OC_CALL:
    case Opcode::OC_CLOSURE:
    case Opcode::OC_LOCAL1:
    case Opcode::OC_SETL1:
    case Opcode::OC_SETG1:
    case Opcode::OC_GLOBAL1:
    case Opcode::OC_INT1:
    case Opcode::OC_POPN:  index = m_disassembleArgs(s, program, code, index, "1  "); break;

    case Opcode::OC_CONST1:
    case Opcode::OC_FUNC1: index = m_disassembleArgs(s, program, code, index, "1C  "); break;

    case Opcode::OC_INT4:
    case Opcode::OC_LOCAL4:
    case Opcode::OC_CALL4:   
    case Opcode::OC_CLOSURE4:
    case Opcode::OC_POPN4:   
    case Opcode::OC_LINE4:   
    case Opcode::OC_JT4:     
    case Opcode::OC_JN4:     
    case Opcode::OC_LIST4: 
    case Opcode::OC_ARRAY4:
    case Opcode::OC_APPEND4:
    case Opcode::OC_GLOBAL4:
    case Opcode::OC_SETG4:
    case Opcode::OC_JMP4:    
    case Opcode::OC_PROC4:  
    case Opcode::OC_SETL4: index = m_disassembleArgs(s, program, code, index, "4 "); break;

    case Opcode::OC_FUNC:
    case Opcode::OC_CONST: index = m_disassembleArgs(s, program, code, index, "4C "); break;

    case Opcode::OC_LINE:
    case Opcode::OC_JT:
    case Opcode::OC_JN:
    case Opcode::OC_LIST:
    case Opcode::OC_APPEND:
    case Opcode::OC_ARRAY:
    case Opcode::OC_JMP:
    case Opcode::OC_LOCAL2:
    case Opcode::OC_SETL2:
    case Opcode::OC_GLOBAL2:
    case Opcode::OC_SETG2:
    case Opcode::OC_INT2:
    case Opcode::OC_PROC:  index = m_disassembleArgs(s, program, code, index, "2 "); break;

    case Opcode::OC_CONST2:
    case Opcode::OC_FUNC2: index = m_disassembleArgs(s, program, code, index, "2C "); break;
    }
    s << '\n';

    return index;
}

int Disassembler::m_disassembleArgs(std::ostream &s, Code *program, const char *code, int index, const char *pattern)
{
	std::string  comment;
    char         ch      = 0;
    int          val     = 0;
    for (int i = 0, sz = strlen(pattern); i < sz; i++)
    {
        ch = pattern[i];
        if (i != 0)
            s << ' ';
        switch (ch)
        {
        case ' ' : s << "      ";                                                     break;
        case '1' : append(s, val = (int)code[index],           6); index++;         break;
        case '2' : append(s, val = m_short(code, index),       6); index += 2;      break;
        case '4' : append(s, val = m_int(code, index),         6); index += 4;      break;
        case 'C' : m_appendComment(comment, m_getConstComment(program, val).c_str()); break;
        default: index = -1; break;
        }
    }
    s << comment;
    return index;
}

std::string &Disassembler::m_appendComment(std::string &s, const char *comment)
{
    if (comment && strlen(comment) != 0)
    {
        if (s.size() == 0)
            s += ';';
        s += ' ';
		s += comment;
    }
    return s;
}

std::string Disassembler::m_writeCodeBytes(Code *program, const char *lineHeader)
{
    ConstantPool                   *pool = program->pool;
    FunctionTable::map_t::iterator  it   = program->function_map().begin(),
		                            end  = program->function_map().end();
    FunctionHandle                 *f    = 0;
	std::ostringstream              s;
	for (; it != end; ++it)
    {
        f = (*it).second;
        s << '\n' << lineHeader << m_functionHeaderComment(program, f)
		  << bytes_to_string(f->code, lineHeader);
    }
    return s.str();
}

std::string Disassembler::m_functionHeaderComment(Code *program, FunctionHandle *f)
{
    ConstantPool *pool = program->pool;
	std::ostringstream s;
    s << " function '"
	  << (f->id == -1 ? "MAIN" : Object::str(pool->constant(f->id)))
	  << "' " << "id=" << f->id << ' ' << "localsize="
	  << f->localsize << ' ' << "stacksize=" << f->stacksize;
    return s.str();
}

std::string Disassembler::m_getConstComment(Code *program, int id)
{
    Object *u = program->pool->constant(id);
	if (!u)
		return "=???";
    else if (Types::cast_symbol_or_string(u))
        return "=\"" + _escape(Object::str(u)) + "\""; // $TODO, convert this string to escape \n \t \r \f etc..
	else if (Object::cast_const_function(u))
	    return "=fn:" + std::string(((ConstFunction *)u)->name());
    else if (Object::cast_const_list(u))
        return "=fn:" + std::string(((ConstFunction *)u)->name());
    else return "=" + _escape(Object::str(u));
}