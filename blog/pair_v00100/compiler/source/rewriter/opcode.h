#ifndef _OPCODE_H
#define _OPCODE_H

#include <string>
#include <vector>
#include <string.h>

struct Instruction
{
    unsigned char code[6];
    unsigned      address;
    int           target;
    int           target_index;

    void print() const;
};

class Opcode
{
public:
    enum OC
	{
         OC_LINE    = 1,
         OC_HEAD    = 2,
         OC_TAIL    = 3,
         OC_LOCAL4  = 4,
         OC_CONST   = 5,
         OC_JT      = 6,
         OC_JN      = 7,
         OC_JMP     = 8,
         OC_CALL    = 9,
         OC_TRUE    = 10,
         OC_NIL     = 11,
         OC_YIELD   = 12,
         OC_BREAK   = 13,
         OC_END     = 14,
         OC_LET4    = 15,
         OC_PAIR    = 16,
         OC_DUP     = 17,
         OC_POP     = 18,
         OC_POPN    = 19,
         OC_PROC    = 20,
         OC_RET     = 21,
         OC_EQ      = 22,
         OC_EQL     = 23,
         OC_EQUAL   = 24,
         OC_NOP     = 25,
         OC_TRACE   = 26,

         OC_INT4    = 27,
         OC_INT2    = 28,
         OC_INT1    = 29,

         OC_CALL4   = 30,
         OC_POPN4   = 31,
         OC_LINE4   = 32,
         OC_JT4     = 33,
         OC_JN4     = 34,
         OC_LIST4   = 35,

         OC_CONTIN  = 36,
         OC_RESUME  = 37,
         OC_STACKN  = 38, // TODO convert local compilation/execution to use stackn for local variables where possible

         OC_ADD     = 43,
         OC_SUB     = 44,
         OC_MUL     = 45,
         OC_DIV     = 46,
         OC_MOD     = 51,

         OC_LT      = 57,
         OC_LTE     = 58,
         OC_GT      = 59,
         OC_GTE     = 60,

         OC_APPEND4 = 61,
         OC_JMP4    = 62,
         OC_PROC4   = 63,

         OC_NEXTP   = 77,
         OC_NEXT    = 78,
         OC_ITER    = 81,

         OC_CONST1  = 82,
         OC_CONST2  = 83,
         OC_FUNC1   = 84,
         OC_FUNC2   = 85,

         OC_TOSTR   = 87, // type conversion
         OC_TOBOOL  = 88,
         OC_TOBYTE  = 89,
         OC_TOCHAR  = 90,
         OC_TOSHT   = 91,
         OC_TOINT   = 92,
         OC_TOLONG  = 93,
         OC_TOFLT   = 94,
         OC_TODBL   = 95,

         OC_APPEND  = 96,
         OC_CPLIST  = 97,
         OC_CPTREE  = 98,
         OC_LIST    = 99,

         OC_GETENV  = 100,
         OC_PUTENV  = 101,

         OC_FUNC    = 102,
         OC_APPLY   = 103,

         OC_LOCAL1  = 104,
         OC_LOCAL2  = 105,

         OC_LET1    = 106,
         OC_LET2    = 107,

         OC_ARGS    = 108,
         OC_LENGTH  = 109,
         OC_NOT     = 110,
         OC_NTH     = 111,
         OC_TYPE    = 112,

         OC_ASSOCP  = 113,
         OC_ATOMP   = 114,
         OC_INTP    = 115,
         OC_ITERP   = 116,
         OC_NILP    = 117,
         OC_NUMP    = 118,
         OC_OFTYPEP = 119,
         OC_PAIRP   = 120,
         OC_REALP   = 121,
         OC_STRINGP = 122,
         OC_SYMBOLP = 123,
         OC_USERP   = 124,
         OC_TYPEP   = 125
    };

    static std::string disassemble(const unsigned char *code, unsigned sz);
    static const char *name(Opcode::OC opcode);
    static int         width(Opcode::OC opcode);
    static int         width(unsigned char opcode) { return width((Opcode::OC)opcode); }
};

unsigned        instruction_count(const unsigned char *code, unsigned sz);
void            instructions(const unsigned char *code, unsigned sz, std::vector<Instruction> &instructions);
void            print(const std::vector<Instruction> &ins);
void            compress(Instruction &i);
void            compress(std::vector<Instruction> &ins);
void            expand(Instruction &i);
void            expand(std::vector<Instruction> &ins);
unsigned        size(const std::vector<Instruction> &ins);
unsigned char  *write(const std::vector<Instruction> &ins);
inline unsigned width(const Instruction &i) { return Opcode::width(*i.code); }

unsigned char  *_expand(const unsigned char *code, unsigned sz, unsigned *outsz);

#endif  // _OPCODE_H