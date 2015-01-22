// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "opcode.h"
#include "object.h"

unsigned Opcode::width(unsigned oc)
{
    switch (oc)
    {
    case Opcode::OC_CALL:
    case Opcode::OC_CLOSURE:
    case Opcode::OC_LOCAL1:
    case Opcode::OC_GLOBAL1:
    case Opcode::OC_SETL1:
    case Opcode::OC_SETG1:
    case Opcode::OC_INT1:
    case Opcode::OC_CONST1:
    case Opcode::OC_FUNC1:
    case Opcode::OC_POPN:   return 1 + 1;

    case Opcode::OC_CLOSURE4:
    case Opcode::OC_LOCAL4:
    case Opcode::OC_GLOBAL4:
    case Opcode::OC_SETL4:
    case Opcode::OC_SETG4:
    case Opcode::OC_LINK_GLOBAL:
    case Opcode::OC_LINK_SETG:
    case Opcode::OC_LINK_FUNC:
    case Opcode::OC_LINK_CONST:
    case Opcode::OC_INT4:
    case Opcode::OC_CALL4:
    case Opcode::OC_POPN4:
    case Opcode::OC_LINE4:
    case Opcode::OC_JT4:
    case Opcode::OC_JN4:   
    case Opcode::OC_LIST4: 
    case Opcode::OC_APPEND4:
    case Opcode::OC_ARRAY4:
    case Opcode::OC_JMP4:
    case Opcode::OC_PROC4:
    case Opcode::OC_FUNC:
    case Opcode::OC_CONST:  return 1 + 4;

    case Opcode::OC_LINE:
    case Opcode::OC_JT:
    case Opcode::OC_JN:
    case Opcode::OC_LIST:
    case Opcode::OC_APPEND:
    case Opcode::OC_ARRAY:
    case Opcode::OC_JMP:
    case Opcode::OC_LOCAL2:
    case Opcode::OC_GLOBAL2:
    case Opcode::OC_SETL2:
    case Opcode::OC_SETG2:
    case Opcode::OC_INT2:
    case Opcode::OC_CONST2:
    case Opcode::OC_FUNC2:
    case Opcode::OC_PROC:   return 1 + 2;

    default:                return 1;
    }
}

struct Instruction
{
    unsigned char code[6];
    unsigned      address;
    int           target;
    int           target_index;
};

static Opcode::OC _expanded(Opcode::OC oc)
{
    switch (oc)
    {
    case Opcode::OC_CALL:    return Opcode::OC_CALL4;
    case Opcode::OC_LOCAL1:  return Opcode::OC_LOCAL4;
    case Opcode::OC_SETL1:   return Opcode::OC_SETL4;
    case Opcode::OC_INT1:    return Opcode::OC_INT4;
    case Opcode::OC_CONST1:  return Opcode::OC_CONST;
    case Opcode::OC_FUNC1:   return Opcode::OC_FUNC;
    case Opcode::OC_POPN:    return Opcode::OC_POPN4;
    case Opcode::OC_LINE:    return Opcode::OC_LINE4;
    case Opcode::OC_JT:      return Opcode::OC_JT4;
    case Opcode::OC_JN:      return Opcode::OC_JN4;
    case Opcode::OC_LIST:    return Opcode::OC_LIST4;
    case Opcode::OC_APPEND:  return Opcode::OC_APPEND4;
    case Opcode::OC_JMP:     return Opcode::OC_JMP4;
    case Opcode::OC_LOCAL2:  return Opcode::OC_LOCAL4; 
    case Opcode::OC_SETL2:   return Opcode::OC_SETL4;
    case Opcode::OC_INT2:    return Opcode::OC_INT4;
    case Opcode::OC_CONST2:  return Opcode::OC_CONST;
    case Opcode::OC_FUNC2:   return Opcode::OC_FUNC;
    case Opcode::OC_PROC:    return Opcode::OC_PROC4;
    case Opcode::OC_SETG1:   return Opcode::OC_SETG4;
    case Opcode::OC_SETG2:   return Opcode::OC_SETG4;
    case Opcode::OC_GLOBAL1: return Opcode::OC_GLOBAL4;
    case Opcode::OC_GLOBAL2: return Opcode::OC_GLOBAL4;
    case Opcode::OC_ARRAY:   return Opcode::OC_ARRAY4;
    }
    return oc;
}

static bool _signed(Opcode::OC oc)
{
    switch (oc)
    {
    case Opcode::OC_INT1:   
    case Opcode::OC_INT2:    
    case Opcode::OC_FUNC1:
    case Opcode::OC_FUNC2:    
    case Opcode::OC_JT:   
    case Opcode::OC_JN:
    case Opcode::OC_JMP: return true;
    }
    return false;
}

inline static unsigned  width(const Instruction &i)                  { return Opcode::width(*i.code); }
inline static int      _int   (const unsigned char *code, int index) { return *((int   *)(code + index)); }
inline static short    _ushort(const unsigned char *code, int index) { return *((unsigned short *)(code + index)); }
inline static short    _short (const unsigned char *code, int index) { return *((short *)(code + index)); }
inline static bool     _is_jump2(unsigned char c)                    { Opcode::OC oc = (Opcode::OC)c; return oc == Opcode::OC_JT || oc == Opcode::OC_JN || oc == Opcode::OC_JMP; }
inline static bool     _is_jump4(unsigned char c)                    { Opcode::OC oc = (Opcode::OC)c; return oc == Opcode::OC_JT4 || oc == Opcode::OC_JN4 || oc == Opcode::OC_JMP4; }
inline static bool     _is_jump(unsigned char c)                     { return _is_jump2(c) || _is_jump4(c); }

unsigned char *expand(const unsigned char *code, unsigned sz, unsigned *outsize)
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
        Opcode::OC oc = (Opcode::OC)*ins.code, oc_expanded = _expanded(oc);
        unsigned w = width(ins);
        if (oc != oc_expanded) 
        {
            int v = 0;
            // width 1 and 5 (there are none of length 4) are ok
            if (_signed(oc)) v = w == 3 ? _short (ins.code, 1) : (char)         ins.code[1];
            else             v = w == 3 ? _ushort(ins.code, 1) : (unsigned char)ins.code[1];
            *ins.code = oc_expanded;
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
    unsigned char *out = new unsigned char [*outsize = size];
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