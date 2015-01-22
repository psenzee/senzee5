#pragma warning(disable: 4786)

#include "opcode.h"
#include <string.h>
#include <vector>

struct Instruction
{
    unsigned char code[6];
    unsigned      address;
    bool          jump;
    int           target;
    int           target_index;
};

inline unsigned width(const Instruction &i) { return Opcode::width(*i.code); }

inline static int   _int  (const unsigned char *code, int index) { return *((int   *)(code + index)); }
inline static short _short(const unsigned char *code, int index) { return *((short *)(code + index)); }

static unsigned instruction_count(const unsigned char *code, unsigned sz)
{
    unsigned count = 0;
    for (unsigned i = 0; i < sz;)
    {
        i += Opcode::width(code[i]);
        count++;
    }
    return count;
}

static unsigned size(const Instruction *ins, unsigned count)
{
    unsigned sz = 0;
    for (unsigned i = 0; i < count; ++i)
        sz += width(ins[i]);
    return sz;
}

static unsigned char *write(const Instruction *instructions, unsigned count)
{
    unsigned char *buffer = new unsigned char [size(instructions, count)];
    unsigned size = 0;
    for (unsigned i = 0; i < count; ++i)
    {
        const Instruction &ins = instructions[i];
        memcpy(buffer + size, ins.code, width(ins));
        size += width(ins);
    }
    return buffer;
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

static unsigned insert(const unsigned char *code, unsigned index, Instruction *instructions, unsigned at)
{
    Opcode::OC oc = (Opcode::OC)code[index];
    unsigned w = Opcode::width(oc);
    Instruction &ins = instructions[at];
    ins.address = index;
    ins.target_index = 0;
    memcpy(ins.code, code + index, w);
    if ((ins.jump = (oc == Opcode::OC_JT || oc == Opcode::OC_JN || oc == Opcode::OC_JMP)))
        ins.target = _short(code, index + 1);
    else if ((ins.jump = (oc == Opcode::OC_JT4 || oc == Opcode::OC_JN4 || oc == Opcode::OC_JMP4)))
        ins.target = _int(code, index + 1);
    return index + w;
}

static int find_index(const Instruction *ins, unsigned count, int i, unsigned absolute_address, int direction)
{
    for (; i >= 0 && i < (int)count && ins[i].address != absolute_address; count++)
        i += direction;
    return (i < 0 || i >= (int)count) ? -1 : i;
}

static bool resolve_target_indices(Instruction *instructions, unsigned count)
{
    for (size_t i = 0; i < count; ++i)
    {
        Instruction &ins = instructions[i];
        if (ins.jump && ins.target)
        {
            ins.target_index = find_index(instructions, count, (int)i, ins.address + ins.target + width(ins), ins.target > 0 ? 1 : -1);
            if (ins.target_index == -1)
            {
                printf("correct target index not found for jump (instruction #%u, address #%u, target #%d, target index #%d)\n", i, ins.address, ins.target, ins.target_index);
                return false; // error!
            }
        }
    }
    return true;
}

static void resolve_addresses(Instruction *instructions, unsigned count)
{
    unsigned address = 0;
    for (unsigned i = 0; i < count; ++i)
    {
        Instruction &ins = instructions[i];
        ins.address = address;
        address += width(ins);
    }
}

static void resolve_jumps(Instruction *instructions, unsigned count)
{
    for (unsigned i = 0; i < count; ++i)
    {
        Instruction &ins = instructions[i];
        if (ins.jump)
            ins.target = instructions[ins.target_index].address - instructions[i + 1].address;
    }
}

static unsigned instructions(const unsigned char *code, unsigned sz, Instruction *ins)
{
    unsigned count = 0;
    for (unsigned i = 0; i < sz; count++)
        i = insert(code, i, ins, count);
    resolve_target_indices(ins, count);
    return count;
}

static Opcode::OC _expanded(Opcode::OC oc)
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

static void expand(Instruction &i)
{
    Opcode::OC oc = (Opcode::OC)*i.code;
    unsigned w = width(i);
    if (w != 2 && w != 3) return; // 1 and 5 (there are none of length 4) are ok
    int v = w == 3 ? _short(i.code, 1) : i.code[1]; // careful: might have sign issues in some cases!
    *i.code = _expanded(oc);
    memcpy(i.code + 1, &v, 4);
}

static void expand(Instruction *ins, unsigned count)
{
    for (unsigned i = 0; i < count; ++i)
        expand(ins[i]);
    resolve_addresses(ins, count);
    resolve_jumps(ins, count);
}

unsigned char *expand(const unsigned char *code, unsigned sz)
{
    unsigned count = instruction_count(code, sz);
    Instruction *ins = new Instruction [count];
    instructions(code, sz, ins);
    expand(ins, count);
    unsigned char *out = write(ins, count);
    delete [] ins;
    return out;
}