// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _OPCODE_H
#define _OPCODE_H

class Opcode
{
public:
    enum OC
    {
        OC_LINE        = 1,
        OC_HEAD        = 2,
        OC_TAIL        = 3,
        OC_LOCAL4      = 4,
        OC_CONST       = 5,
        OC_JT          = 6,
        OC_JN          = 7,
        OC_JMP         = 8,
        OC_CALL        = 9,
        OC_TRUE        = 10,
        OC_NIL         = 11,
        OC_YIELD       = 12,
        OC_BREAK       = 13,
        OC_END         = 14,
        OC_SETL4       = 15,
        OC_PAIR        = 16,
        OC_DUP         = 17,
        OC_POP         = 18,
        OC_POPN        = 19,
        OC_PROC        = 20,
        OC_RET         = 21,
        OC_EQ          = 22,
        OC_EQL         = 23,
        OC_EQUAL       = 24,
        OC_NOP         = 25,
        OC_TRACE       = 26,

        OC_INT4        = 27,
        OC_INT2        = 28,
        OC_INT1        = 29,

        OC_CALL4       = 30,
        OC_POPN4       = 31,
        OC_LINE4       = 32,
        OC_JT4         = 33,
        OC_JN4         = 34,
        OC_LIST4       = 35,

        OC_CONTIN      = 36,
        OC_RESUME      = 37,
        OC_STACKN      = 38,

        OC_CLOSURE     = 39,
        OC_CLOSURE4    = 40,

        OC_SHR         = 41,
        OC_SHL         = 42,

        OC_ADD         = 43,
        OC_SUB         = 44,
        OC_MUL         = 45,
        OC_DIV         = 46,

        OC_BITAND      = 47,
        OC_BITOR       = 48,
        OC_BITXOR      = 49,
        OC_BITNOT      = 50,

        OC_MOD         = 51,

        OC_LINK_CONST  = 52,
        OC_LINK_FUNC   = 53,
        OC_LINK_GLOBAL = 54,
        OC_LINK_SETG   = 55,

        OC_ARRAY       = 56,

        OC_LT          = 57,
        OC_LTE         = 58,
        OC_GT          = 59,
        OC_GTE         = 60,

        OC_APPEND4     = 61,
        OC_JMP4        = 62,
        OC_PROC4       = 63,
        OC_ARRAY4      = 64,

        OC_SETNTH      = 65,

        OC_NEXTP       = 77,
        OC_NEXT        = 78,
        OC_ITER        = 81,

        OC_CONST1      = 82,
        OC_CONST2      = 83,
        OC_FUNC1       = 84,
        OC_FUNC2       = 85,

        OC_GLOBAL4     = 86,
        OC_SETG4       = 87,

        OC_NOTNILP     = 88,
        OC_COND        = 89,

        OC_APPEND      = 96,
        OC_CPLIST      = 97,
        OC_CPTREE      = 98,
        OC_LIST        = 99, 

        OC_GLOBAL2     = 100,
        OC_SETG2       = 101,

        OC_FUNC        = 102,
        OC_APPLY       = 103,

        OC_LOCAL1      = 104,
        OC_LOCAL2      = 105,

        OC_SETL1       = 106,
        OC_SETL2       = 107,

        OC_ARGS        = 108,

        OC_LENGTH      = 109,
        OC_NOT         = 110,
        OC_NTH         = 111,
        OC_TYPE        = 112,

        OC_ASSOCP      = 113,
        OC_ATOMP       = 114,
        OC_INTP        = 115,
        OC_ITERP       = 116,
        OC_NILP        = 117,
        OC_NUMP        = 118,
        OC_OFTYPEP     = 119,
        OC_PAIRP       = 120,
        OC_REALP       = 121,
        OC_STRINGP     = 122,
        OC_SYMBOLP     = 123,
        OC_USERP       = 124,
        OC_TYPEP       = 125,

        OC_GLOBAL1     = 126,
        OC_SETG1       = 127,

        OC_SETL_0      = 128,
        OC_SETL_1      = 129,
        OC_SETL_2      = 130,
        OC_SETL_3      = 131,
        OC_SETL_4      = 132,
        OC_SETL_5      = 133,
        OC_SETL_6      = 134,
        OC_SETL_7      = 135,
        OC_SETL_8      = 136,
        OC_SETL_9      = 137,
        OC_SETL_10     = 138,
        OC_SETL_11     = 139,
        OC_SETL_12     = 140,
        OC_SETL_13     = 141,
        OC_SETL_14     = 142,
        OC_SETL_15     = 143,

        OC_LOCAL_0     = 144,
        OC_LOCAL_1     = 145,
        OC_LOCAL_2     = 146,
        OC_LOCAL_3     = 147,
        OC_LOCAL_4     = 148,
        OC_LOCAL_5     = 149,
        OC_LOCAL_6     = 150,
        OC_LOCAL_7     = 151,
        OC_LOCAL_8     = 152,
        OC_LOCAL_9     = 153,
        OC_LOCAL_10    = 154,
        OC_LOCAL_11    = 155,
        OC_LOCAL_12    = 156,
        OC_LOCAL_13    = 157,
        OC_LOCAL_14    = 158,
        OC_LOCAL_15    = 159
    };
	
    static unsigned width(unsigned opcode);
};

#endif  // _OPCODE_H