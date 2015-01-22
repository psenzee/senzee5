// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "opcode.h"

const char *Opcode::name(unsigned char opcode)
{
    switch (opcode)
    {
    case OC_LINE:     return "LINE    ";
    case OC_TRACE:    return "TRACE   ";
    case OC_PAIR:     return "PAIR    ";
    case OC_DUP:      return "DUP     ";
    case OC_POP:      return "POP     ";
    case OC_POPN:     return "POPN    ";
    case OC_HEAD:     return "HEAD    ";
    case OC_TAIL:     return "TAIL    ";
    case OC_LOCAL4:   return "LOCAL   ";
    case OC_SETL4:    return "SETL    ";
    case OC_CONST:    return "CONST   ";
    case OC_JT:       return "JT      ";
    case OC_JN:       return "JN      ";
    case OC_ARRAY:    return "ARRAY   ";
    case OC_ARRAY4:   return "ARRAY   ";
    case OC_SETNTH:   return "SETNTH  ";

    case OC_JMP:      return "JMP     ";
    case OC_CALL:     return "CALL    ";
    case OC_CLOSURE:  return "CLOSURE ";
    case OC_CLOSURE4: return "CLOSURE ";
    case OC_TRUE:     return "TRUE    ";
    case OC_NIL:      return "NIL     ";
    case OC_YIELD:    return "YIELD   ";
    case OC_BREAK:    return "BREAK   ";
    case OC_END:      return "END     ";
    case OC_RET:      return "RET     ";
    case OC_EQ:       return "EQ      ";
    case OC_EQL:      return "EQL     ";
    case OC_EQUAL:    return "EQUAL   ";
    case OC_NOP:      return "NOP     ";
    case OC_PROC:     return "PROC    ";

    case OC_CONTIN:   return "CONTIN  ";
    case OC_RESUME:   return "RESUME  ";
    case OC_STACKN:   return "STACKN  ";

    case OC_SHR:      return "SHR     ";
    case OC_SHL:      return "SHL     ";

    case OC_ADD:      return "ADD     ";
    case OC_SUB:      return "SUB     ";
    case OC_MUL:      return "MUL     ";
    case OC_DIV:      return "DIV     ";
    case OC_MOD:      return "MOD     ";

    case OC_BITAND:   return "BITAND  ";
    case OC_BITOR:    return "BITOR   ";
    case OC_BITXOR:   return "BITXOR  ";
    case OC_BITNOT:   return "BITNOT  ";

    case OC_LT:       return "LT      ";
    case OC_LTE:      return "LTE     ";
    case OC_GT:       return "GT      ";
    case OC_GTE:      return "GTE     ";

    case OC_NOTNILP:  return "NOTNILP ";
    case OC_COND:     return "COND    ";

    case OC_NEXTP:    return "NEXTP   ";
    case OC_NEXT:     return "NEXT    ";

    case OC_ITER:     return "ITER    ";

    case OC_APPEND:   return "APPEND  ";
    case OC_CPLIST:   return "CPLIST  ";
    case OC_CPTREE:   return "CPTREE  ";
    case OC_LIST:     return "LIST    ";

    case OC_GLOBAL1:  return "GLOBAL  ";
    case OC_SETG1:    return "SETG    ";
    case OC_GLOBAL2:  return "GLOBAL  ";
    case OC_SETG2:    return "SETG    ";
    case OC_GLOBAL4:  return "GLOBAL  ";
    case OC_SETG4:    return "SETG    ";

    case OC_FUNC:     return "FUNC    ";
    case OC_APPLY:    return "APPLY   ";

    case OC_LOCAL1:   return "LOCAL   ";
    case OC_LOCAL2:   return "LOCAL   ";

    case OC_SETL1:    return "SETL    ";
    case OC_SETL2:    return "SETL    ";

    case OC_CONST1:   return "CONST   ";
    case OC_CONST2:   return "CONST   ";

    case OC_FUNC1:    return "FUNC    ";
    case OC_FUNC2:    return "FUNC    ";

    case OC_ARGS:     return "ARGS    ";

    case OC_LENGTH:   return "LENGTH  ";
    case OC_NOT:      return "NOT     ";
    case OC_NTH:      return "NTH     ";
    case OC_TYPE:     return "TYPE    ";

    case OC_ASSOCP:   return "ASSOCP  ";
    case OC_ATOMP:    return "ATOMP   ";
    case OC_INTP:     return "INTP    ";
    case OC_ITERP:    return "ITERP   ";
    case OC_NILP:     return "NILP    ";
    case OC_NUMP:     return "NUMP    ";
    case OC_OFTYPEP:  return "OFTYPEP ";
    case OC_PAIRP:    return "PAIRP   ";
    case OC_REALP:    return "REALP   ";
    case OC_STRINGP:  return "STRINGP ";
    case OC_SYMBOLP:  return "SYMBOLP ";
    case OC_USERP:    return "USERP   ";

    case OC_INT4:     return "INT     ";
    case OC_INT2:     return "INT     ";
    case OC_INT1:     return "INT     ";

    case OC_CALL4:    return "CALL    ";
    case OC_POPN4:    return "POPN    ";
    case OC_LINE4:    return "LINE    ";
    case OC_JT4:      return "JT      ";
    case OC_JN4:      return "JN      ";
    case OC_LIST4:    return "LIST    ";
    case OC_APPEND4:  return "APPEND  ";
    case OC_JMP4:     return "JMP     ";
    case OC_PROC4:    return "PROC    ";

    case OC_SETL_0:   return "SETL    0 ";
    case OC_SETL_1:   return "SETL    1 ";
    case OC_SETL_2:   return "SETL    2 ";
    case OC_SETL_3:   return "SETL    3 ";
    case OC_SETL_4:   return "SETL    4 ";
    case OC_SETL_5:   return "SETL    5 ";
    case OC_SETL_6:   return "SETL    6 ";
    case OC_SETL_7:   return "SETL    7 ";
    case OC_SETL_8:   return "SETL    8 ";
    case OC_SETL_9:   return "SETL    9 ";
    case OC_SETL_10:  return "SETL    10";
    case OC_SETL_11:  return "SETL    11";
    case OC_SETL_12:  return "SETL    12";
    case OC_SETL_13:  return "SETL    13";
    case OC_SETL_14:  return "SETL    14";
    case OC_SETL_15:  return "SETL    15";

    case OC_LOCAL_0:  return "LOCAL   0 ";
    case OC_LOCAL_1:  return "LOCAL   1 ";
    case OC_LOCAL_2:  return "LOCAL   2 ";
    case OC_LOCAL_3:  return "LOCAL   3 ";
    case OC_LOCAL_4:  return "LOCAL   4 ";
    case OC_LOCAL_5:  return "LOCAL   5 ";
    case OC_LOCAL_6:  return "LOCAL   6 ";
    case OC_LOCAL_7:  return "LOCAL   7 ";
    case OC_LOCAL_8:  return "LOCAL   8 ";
    case OC_LOCAL_9:  return "LOCAL   9 ";
    case OC_LOCAL_10: return "LOCAL   10";
    case OC_LOCAL_11: return "LOCAL   11";
    case OC_LOCAL_12: return "LOCAL   12";
    case OC_LOCAL_13: return "LOCAL   13";
    case OC_LOCAL_14: return "LOCAL   14";
    case OC_LOCAL_15: return "LOCAL   15";

    default:     return "*BAD-OP*";
    }
}

unsigned Opcode::width(unsigned char oc)
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