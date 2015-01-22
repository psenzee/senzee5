#pragma warning(disable: 4786)

#include "opcode.h"

#include <math.h>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>

inline static int   _int  (const unsigned char *code, int index) { return *((int   *)(code + index)); }
inline static short _short(const unsigned char *code, int index) { return *((short *)(code + index)); }

static inline bool _is_jump2(unsigned char c)
{
    Opcode::OC oc = (Opcode::OC)c;
    return oc == Opcode::OC_JT || oc == Opcode::OC_JN || oc == Opcode::OC_JMP;
}

static inline bool _is_jump4(unsigned char c)
{
    Opcode::OC oc = (Opcode::OC)c;
    return oc == Opcode::OC_JT4 || oc == Opcode::OC_JN4 || oc == Opcode::OC_JMP4;
}

static inline bool _is_jump(unsigned char c)
{
    return _is_jump2(c) || _is_jump4(c);
}

static inline int _jump_target(const unsigned char *code)
{
    if (_is_jump2(*code))
        return _short(code, 1);
    if (_is_jump4(*code))
        return _int(code, 1);
    return 0;
}

static int disassemble_args(std::ostream &s, const unsigned char *code, int index, const char *pattern);
static int disassemble_line(const unsigned char *code, int index, std::ostream &s, bool address = true);

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

static std::string justify(const char *cs, size_t fieldsz, End justify, bool trunc)
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

void Instruction::print() const
{
    std::ostringstream s;
    disassemble_line(code, 0, s, false);
    if (_is_jump(*code) && target_index != 0)
        s << "; target index " << target_index;
    printf("%s\n", s.str().c_str());
}

std::string Opcode::disassemble(const unsigned char *code, unsigned sz)
{
    std::ostringstream s;
    for (unsigned i = 0; i < sz;)
    {
        i = disassemble_line(code, i, s);
        s << '\n';
    }
    return s.str();
}

unsigned instruction_count(const unsigned char *code, unsigned sz)
{
    unsigned count = 0;
    for (unsigned i = 0; i < sz;)
    {
        i += Opcode::width(code[i]);
        count++;
    }
    return count;
}

unsigned size(const std::vector<Instruction> &ins)
{
    unsigned sz = 0;
    for (std::vector<Instruction>::const_iterator i = ins.begin(), e = ins.end(); i != e; ++i)
        sz += width(*i);
    return sz;
}

unsigned char *write(const std::vector<Instruction> &ins)
{
    unsigned char *buffer = new unsigned char [size(ins)];
    unsigned size = 0;
    for (std::vector<Instruction>::const_iterator i = ins.begin(), e = ins.end(); i != e; ++i)
    {
        memcpy(buffer + size, (*i).code, width(*i));
        size += width(*i);
    }
    return buffer;
}

void print(const std::vector<Instruction> &ins)
{
    printf("\ninstruction count %u\n", ins.size());
    unsigned count = 0, size = 0;
    for (std::vector<Instruction>::const_iterator i = ins.begin(), e = ins.end(); i != e; ++i)
    {
        printf("%-4u %-5u", count++, (*i).address);
        (*i).print();
        size += width(*i);
    }
    printf("code size %u\n", size);
}

const char *Opcode::name(Opcode::OC opcode)
{
    switch (opcode)
    {
    case OC_LINE:    return "LINE     ";
    case OC_TRACE:   return "TRACE    ";
    case OC_PAIR:    return "PAIR     ";
    case OC_DUP:     return "DUP      ";
    case OC_POP:     return "POP      ";
    case OC_POPN:    return "POPN     ";
    case OC_HEAD:    return "HEAD     ";
    case OC_TAIL:    return "TAIL     ";
    case OC_LOCAL4:  return "LOCAL    ";
    case OC_LET4:    return "LET      ";
    case OC_CONST:   return "CONST    ";
    case OC_JT:      return "JT       ";
    case OC_JN:      return "JN       ";

    case OC_JMP:     return "JMP      ";
    case OC_CALL:    return "CALL     ";
    case OC_TRUE:    return "TRUE     ";
    case OC_NIL:     return "NIL      ";
    case OC_YIELD:   return "YIELD    ";
    case OC_BREAK:   return "BREAK    ";
    case OC_END:     return "END      ";
    case OC_RET:     return "RET      ";
    case OC_EQ:      return "EQ       ";
    case OC_EQL:     return "EQL      ";
    case OC_EQUAL:   return "EQUAL    ";
    case OC_NOP:     return "NOP      ";
    case OC_PROC:    return "PROC     ";

    case OC_CONTIN:  return "CONTIN   ";
    case OC_RESUME:  return "RESUME   ";
    case OC_STACKN:  return "STACKN   ";

    case OC_ADD:     return "ADD      ";
    case OC_SUB:     return "SUB      ";
    case OC_MUL:     return "MUL      ";
    case OC_DIV:     return "DIV      ";
    case OC_MOD:     return "MOD      ";

    case OC_LT:      return "LT       ";
    case OC_LTE:     return "LTE      ";
    case OC_GT:      return "GT       ";
    case OC_GTE:     return "GTE      ";

    case OC_NEXTP:   return "NEXTP    ";
    case OC_NEXT:    return "NEXT     ";

    case OC_ITER:    return "ITER     ";

    case OC_APPEND:  return "APPEND   ";
    case OC_CPLIST:  return "CPLIST   ";
    case OC_CPTREE:  return "CPTREE   ";
    case OC_LIST:    return "LIST     ";

    case OC_GETENV:  return "GETENV   ";
    case OC_PUTENV:  return "PUTENV   ";
    case OC_FUNC:    return "FUNC     ";
    case OC_APPLY:   return "APPLY    ";

    case OC_LOCAL1:  return "LOCAL(1) ";
    case OC_LOCAL2:  return "LOCAL(2) ";

    case OC_LET1:    return "LET(1)   ";
    case OC_LET2:    return "LET(2)   ";

    case OC_CONST1:  return "CONST(1) ";
    case OC_CONST2:  return "CONST(2) ";

    case OC_FUNC1:   return "FUNC(1)  ";
    case OC_FUNC2:   return "FUNC(2)  ";

    case OC_ARGS:    return "ARGS     ";

    case OC_LENGTH:  return "LENGTH   ";
    case OC_NOT:     return "NOT      ";
    case OC_NTH:     return "NTH      ";
    case OC_TYPE:    return "TYPE     ";

    case OC_ASSOCP:  return "ASSOCP   ";
    case OC_ATOMP:   return "ATOMP    ";
    case OC_INTP:    return "INTP     ";
    case OC_ITERP:   return "ITERP    ";
    case OC_NILP:    return "NILP     ";
    case OC_NUMP:    return "NUMP     ";
    case OC_OFTYPEP: return "OFTYPEP  ";
    case OC_PAIRP:   return "PAIRP    ";
    case OC_REALP:   return "REALP    ";
    case OC_STRINGP: return "STRINGP  ";
    case OC_SYMBOLP: return "SYMBOLP  ";
    case OC_USERP:   return "USERP    ";

    case OC_INT4:    return "INT      ";
    case OC_INT2:    return "INT(2)   ";
    case OC_INT1:    return "INT(1)   ";

    case OC_CALL4:   return "CALL(4)  ";
    case OC_POPN4:   return "POPN(4)  ";
    case OC_LINE4:   return "LINE(4)  ";
    case OC_JT4:     return "JT(4)    ";
    case OC_JN4:     return "JN(4)    ";
    case OC_LIST4:   return "LIST(4)  ";
    case OC_APPEND4: return "APPEND(4)";
    case OC_JMP4:    return "JMP(4)   ";
    case OC_PROC4:   return "PROC(4)  ";

    default:         return "*BAD-OP* ";
    }
}

static int disassemble_line(const unsigned char *code, int index, std::ostream &s, bool address)
{
    Opcode::OC oc = (Opcode::OC)code[index];

    if (address)
        append(s, index, 5);

	s << ' ' <<
        justify(("[" + bytes_to_string(code, index, index + Opcode::width(oc), 0) + "]").c_str(),
			20, LEFT, false) << ' ';

    s << Opcode::name(oc);

    index++;

    switch (oc)
    {
    case Opcode::OC_CALL:
    case Opcode::OC_LOCAL1:
    case Opcode::OC_LET1:
    case Opcode::OC_INT1:
    case Opcode::OC_CONST1:
    case Opcode::OC_FUNC1:
    case Opcode::OC_POPN:  index = disassemble_args(s, code, index, "1  "); break;

    case Opcode::OC_INT4:
    case Opcode::OC_LOCAL4:
    case Opcode::OC_CALL4:   
    case Opcode::OC_POPN4:   
    case Opcode::OC_LINE4:   
    case Opcode::OC_JT4:     
    case Opcode::OC_JN4:     
    case Opcode::OC_LIST4:   
    case Opcode::OC_APPEND4: 
    case Opcode::OC_JMP4:    
    case Opcode::OC_PROC4:   
    case Opcode::OC_LET4:  index = disassemble_args(s, code, index, "4 "); break;

    case Opcode::OC_FUNC:
    case Opcode::OC_CONST: index = disassemble_args(s, code, index, "c "); break;

    case Opcode::OC_LINE:
    case Opcode::OC_JT:
    case Opcode::OC_JN:
    case Opcode::OC_LIST:
    case Opcode::OC_APPEND:
    case Opcode::OC_JMP:
    case Opcode::OC_LOCAL2:
    case Opcode::OC_LET2:
    case Opcode::OC_INT2:
    case Opcode::OC_CONST2:
    case Opcode::OC_FUNC2:
    case Opcode::OC_PROC:  index = disassemble_args(s, code, index, "2 "); break;
    }

    return index;
}

int Opcode::width(Opcode::OC oc)
{
    switch (oc)
    {
    case Opcode::OC_CALL:
    case Opcode::OC_LOCAL1:
    case Opcode::OC_LET1:
    case Opcode::OC_INT1:
    case Opcode::OC_CONST1:
    case Opcode::OC_FUNC1:
    case Opcode::OC_POPN:   return 1 + 1;

    case Opcode::OC_LOCAL4:
    case Opcode::OC_LET4:
    case Opcode::OC_INT4:
    case Opcode::OC_CALL4:
    case Opcode::OC_POPN4:
    case Opcode::OC_LINE4:
    case Opcode::OC_JT4:
    case Opcode::OC_JN4:   
    case Opcode::OC_LIST4: 
    case Opcode::OC_APPEND4:
    case Opcode::OC_JMP4:
    case Opcode::OC_PROC4:
    case Opcode::OC_FUNC:
    case Opcode::OC_CONST:  return 1 + 4;

    case Opcode::OC_LINE:
    case Opcode::OC_JT:
    case Opcode::OC_JN:
    case Opcode::OC_LIST:
    case Opcode::OC_APPEND:
    case Opcode::OC_JMP:
    case Opcode::OC_LOCAL2:
    case Opcode::OC_LET2:
    case Opcode::OC_INT2:
    case Opcode::OC_CONST2:
    case Opcode::OC_FUNC2:
    case Opcode::OC_PROC:   return 1 + 2;

	default:                return 1;
    }
}

static unsigned insert(const unsigned char *code, unsigned index, std::vector<Instruction> &instructions)
{
    Opcode::OC oc = (Opcode::OC)code[index];
    unsigned w = Opcode::width(oc);
    Instruction ins;
    ins.address = index;
    ins.target = 0;
    ins.target_index = 0;
    memcpy(ins.code, code + index, w);
    if (_is_jump2(oc))
        ins.target = _short(code, index + 1);
    if (_is_jump4(oc))
        ins.target = _int(code, index + 1);
    instructions.push_back(ins);
    return index + w;
}

int find_index(const std::vector<Instruction> &instructions, int i, unsigned absolute_address, int direction)
{
    int sz = (int)instructions.size();
    while (i >= 0 && i < sz && instructions[i].address != absolute_address)
        i += direction;
    return (i < 0 || i >= sz) ? -1 : i;
}

bool resolve_target_indices(std::vector<Instruction> &instructions)
{
    for (size_t i = 0, sz = instructions.size(); i < sz; ++i)
    {
        Instruction &ins = instructions[i];
        if (_is_jump(*ins.code) && ins.target)
        {
            ins.target_index = find_index(instructions, (int)i, ins.address + ins.target + width(ins), ins.target > 0 ? 1 : -1);
            if (ins.target_index == -1)
            {
                printf("correct target index not found for jump (instruction #%u, address #%u, target #%d, target index #%d)\n", i, ins.address, ins.target, ins.target_index);
                return false; // error!
            }
        }
    }
    return true;
}

bool verify_jumps(std::vector<Instruction> &instructions)
{
    for (size_t i = 0, sz = instructions.size(); i < sz; ++i)
    {
        Instruction &ins = instructions[i];
        int target_index = 0;
        if (_is_jump(*ins.code) && ins.target)
        {
            target_index = find_index(instructions, (int)i, ins.address + ins.target + width(ins), ins.target > 0 ? 1 : -1);
            if (ins.target_index != target_index)
            {
                printf("correct target index not found for jump (instruction #%u, address #%u, target #%d, target index #%d)\n", i, ins.address, ins.target, ins.target_index);
                printf("  expected: target index #%d; found: target index #%d\n", ins.target_index, target_index);
            }
            if (ins.target != _jump_target(ins.code))
            {
                printf("incorrect target found for jump (instruction #%u, address #%u, target #%d, target index #%d)\n", i, ins.address, ins.target, ins.target_index);
            }
        }
    }
    return true;
}

void resolve_addresses(std::vector<Instruction> &instructions)
{
    unsigned address = 0;
    for (size_t i = 0, sz = instructions.size(); i < sz; ++i)
    {
        Instruction &ins = instructions[i];
        ins.address = address;
        address += width(ins);
    }
}

void resolve_jumps(std::vector<Instruction> &instructions)
{
    for (size_t i = 0, sz = instructions.size(); i < sz; ++i)
    {
        Instruction &ins = instructions[i];
        short target = instructions[ins.target_index].address - instructions[i + 1].address;
        if (_is_jump2(*ins.code))
        {
            ins.target = target;
            memcpy(ins.code + 1, &target, sizeof(short));
        }
        else if (_is_jump4(*ins.code))
            memcpy(ins.code + 1, &(ins.target = target), sizeof(int));
    }
}

void instructions(const unsigned char *code, unsigned sz, std::vector<Instruction> &instructions)
{
    for (unsigned i = 0; i < sz;)
        i = insert(code, i, instructions);
    resolve_target_indices(instructions);
}

void compress(Instruction &i)
{
    Opcode::OC oc = (Opcode::OC)*i.code;
    if (oc != Opcode::OC_CONST && oc != Opcode::OC_FUNC)
        return;
    int v = _int(i.code, 1);
    if (v < 0)
        return;
    else if (v < 256)
    {
        if (oc == Opcode::OC_CONST)
            *i.code = Opcode::OC_CONST1;
        else if (oc == Opcode::OC_FUNC)
            *i.code = Opcode::OC_FUNC1;
        else return;
        i.code[1] = (unsigned char)v;
    }
    else if (v < 65536)
    {
        if (oc == Opcode::OC_CONST)
            *i.code = Opcode::OC_CONST2;
        else if (oc == Opcode::OC_FUNC)
            *i.code = Opcode::OC_FUNC2;
        else return;
        unsigned short vs = v;
        memcpy(i.code + 1, &vs, 2);
    }
}

Opcode::OC _expanded(Opcode::OC oc)
{
    switch (oc)
    {
    case Opcode::OC_CALL:   return Opcode::OC_CALL4;
    case Opcode::OC_LOCAL1: return Opcode::OC_LOCAL4;
    case Opcode::OC_LET1:   return Opcode::OC_LET4;
    case Opcode::OC_INT1:   return Opcode::OC_INT4;
    case Opcode::OC_CONST1: return Opcode::OC_CONST;
    case Opcode::OC_FUNC1:  return Opcode::OC_FUNC;
    case Opcode::OC_POPN:   return Opcode::OC_POPN4;
    case Opcode::OC_LINE:   return Opcode::OC_LINE4;
    case Opcode::OC_JT:     return Opcode::OC_JT4;
    case Opcode::OC_JN:     return Opcode::OC_JN4;
    case Opcode::OC_LIST:   return Opcode::OC_LIST4;
    case Opcode::OC_APPEND: return Opcode::OC_APPEND4;
    case Opcode::OC_JMP:    return Opcode::OC_JMP4;
    case Opcode::OC_LOCAL2: return Opcode::OC_LOCAL4; 
    case Opcode::OC_LET2:   return Opcode::OC_LET4;
    case Opcode::OC_INT2:   return Opcode::OC_INT4;
    case Opcode::OC_CONST2: return Opcode::OC_CONST;
    case Opcode::OC_FUNC2:  return Opcode::OC_FUNC;
    case Opcode::OC_PROC:   return Opcode::OC_PROC4;
    }
    return oc;
}

void expand(Instruction &i)
{
    Opcode::OC oc = (Opcode::OC)*i.code;
    unsigned w = width(i);
    if (w != 2 && w != 3) return; // 1 and 5 (there are none of length 4) are ok
    int v = w == 3 ? _short(i.code, 1) : i.code[1]; // careful: might have sign issues in some cases!
    *i.code = _expanded(oc);
    memcpy(i.code + 1, &v, 4);
}

void compress(std::vector<Instruction> &instructions)
{
    for (std::vector<Instruction>::iterator i = instructions.begin(), e = instructions.end(); i != e; ++i)
        compress(*i);
    resolve_addresses(instructions);
    resolve_jumps(instructions);
    verify_jumps(instructions);
}

void expand(std::vector<Instruction> &instructions)
{
    for (std::vector<Instruction>::iterator i = instructions.begin(), e = instructions.end(); i != e; ++i)
        expand(*i);
    resolve_addresses(instructions);
    resolve_jumps(instructions);
    verify_jumps(instructions);
}

static int disassemble_args(std::ostream &s, const unsigned char *code, int index, const char *pattern)
{
	std::string  comment;
    char         ch      = 0;
    int          val     = 0;
    for (size_t i = 0, sz = strlen(pattern); i < sz; i++)
    {
        ch = pattern[i];
        if (i != 0)
            s << ' ';
        switch (ch)
        {
        case ' ' : s << "      ";                                        break;
        case '1' : append(s, (int)code[index],           6); index++;    break;
        case '2' : append(s, _short(code, index),        6); index += 2; break;
        case '4' : append(s, _int(code, index),          6); index += 4; break;
        case 'c' : append(s, val = _int(code, index),    6); index += 4; /* supposed to have comment */ break;
        default: index = -1; break;
        }
    }
    return index;
}

unsigned char *_expand(const unsigned char *code, unsigned sz, unsigned *outsz)
{
    // get instruction count
    unsigned count = 0;
    for (unsigned i = 0; i < sz; count++) i += Opcode::width(code[i]);
    // create instruction array
    Instruction *instructions = new Instruction [count];
    // create the instruction list
    for (unsigned i = 0, c = 0; i < sz; c++)
    {
        // insert instruction into array
        Opcode::OC oc = (Opcode::OC)code[i];
        unsigned w = Opcode::width(oc);
        Instruction &ins = instructions[c];
        ins.address = i;
        ins.target_index = 0;
        memcpy(ins.code, code + i, w);
        if (_is_jump2(*ins.code))
            ins.target = _short(code, i + 1);
        else if (_is_jump4(*ins.code))
            ins.target = _int(code, i + 1);
        i += w;
    }
    // resolve target indices in each jmp/jn/jt instruction
    for (int i = 0; i < (int)count; ++i)
    {
        Instruction &ins = instructions[i];
        if (_is_jump(*ins.code) && ins.target)
        {
            unsigned absolute_address = ins.address + ins.target + width(ins);
            int direction = ins.target > 0 ? 1 : -1, j = i;
            while (j >= 0 && j < (int)count && instructions[j].address != absolute_address)
                j += direction;
            if (j < 0 || j >= (int)count)
                return 0; // error!!
            ins.target_index = j;
        }
    }
    // expand each 2 and 3 byte instruction to full 5 bytes,
    // resolve addresses & figure size for expanded buffer
    unsigned size = 0;
    for (unsigned i = 0; i < count; ++i)
    {
        // expand each instruction
        Instruction &ins = instructions[i];
        Opcode::OC oc = (Opcode::OC)*ins.code;
        unsigned w = width(ins);
        if (w == 2 || w == 3) // 1 and 5 (there are none of length 4) are ok
        {
            int v = w == 3 ? _short(ins.code, 1) : ins.code[1]; // careful: might have sign issues in some cases!
            *ins.code = _expanded(oc);
            memcpy(ins.code + 1, &v, 4);
        }
        ins.address = size;
        size += width(ins);
    }
    // resolve jumps
    for (unsigned i = 0; i < count; ++i)
    {
        Instruction &ins = instructions[i];
        short target = instructions[ins.target_index].address - instructions[i + 1].address;
        if (_is_jump2(*ins.code))
        {
            ins.target = target;
            memcpy(ins.code + 1, &target, sizeof(short));
        }
        else if (_is_jump4(*ins.code))
            memcpy(ins.code + 1, &(ins.target = target), sizeof(int));
    }
    // write instruction list to out buffer
    unsigned char *out = new unsigned char [(*outsz = size)];
    for (unsigned i = 0, size = 0; i < count; ++i)
    {
        const Instruction &ins = instructions[i];
        memcpy(out + size, ins.code, width(ins));
        size += width(ins);
    }
    // destroy instruction array
    delete [] instructions;
    // return out buffer
    return out;
}