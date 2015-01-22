// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "compilervmtarget.h"
#include "control.h"
#include "data.h"
#include "functions.h"
#include "singleopcode.h"
#include "opcode.h"

CompilerVmTarget *CompilerVmTarget::INSTANCE = new CompilerVmTarget;

std::map<std::string, IElementCompiler *> &CompilerVmTarget::compile_map()
{
    if (m_map.size()) return m_map;

    m_put("quote",                  new QuoteCompilerVm);
    m_put("let",                    new LetCompilerVm);
    m_put("$let",                   new LetCompilerVm);
    m_put("set",                    new SetCompilerVm);
    m_put("define",                 new DefineGlobalCompilerVm);
    
    m_put("if",                     new IfCompilerVm);
    m_put("while",                  new WhileCompilerVm);
    m_put("procedure",              new ProcedureCompilerVm);
    m_put("$scope",                 new ProcedureCompilerVm); // for now, ProcedureCompileVm - may change
    m_put("foreach",                new ForEachCompilerVm);

    m_put("function",               new DefineFunctionCompilerVm);
    m_put("fn",                     new FunctionInstanceCompilerVm);

    m_put("_nop",                   new SingleOpcodeCompilerVm      (Opcode::OC_NOP,     SIDE_EFFECT,                    0, 0));
    m_put("_trace",                 new SingleOpcodeCompilerVm      (Opcode::OC_TRACE,   SIDE_EFFECT,                    0, 0));
    m_put("$cond",                  new SingleOpcodeCompilerVm      (Opcode::OC_COND,    NO_SIDE_EFFECT | PUSHES_RESULT, 3, 3));

    m_put("first",                  new SingleOpcodeCompilerVm      (Opcode::OC_HEAD,    NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("rest",                   new SingleOpcodeCompilerVm      (Opcode::OC_TAIL,    NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("nth",                    new SingleOpcodeCompilerVm      (Opcode::OC_NTH,     NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put("set-nth",                new SingleOpcodeCompilerVm      (Opcode::OC_SETNTH,  SIDE_EFFECT,                    3, 3));
    m_put("pair",                   new SingleOpcodeCompilerVm      (Opcode::OC_PAIR,    NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put("apply",                  new SingleOpcodeCompilerVm      (Opcode::OC_APPLY,   SIDE_EFFECT    | PUSHES_RESULT, 2, 2)); // this may need to be compiled separately
    m_put("args",                   new SingleOpcodeCompilerVm      (Opcode::OC_ARGS,    NO_SIDE_EFFECT | PUSHES_RESULT, 0, 0));
    m_put("equal?",                 new SingleOpcodeCompilerVm      (Opcode::OC_EQUAL,   NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put("continuation",           new SingleOpcodeCompilerVm      (Opcode::OC_CONTIN,  NO_SIDE_EFFECT | PUSHES_RESULT, 0, 0));
    m_put("resume",                 new SingleOpcodeCompilerVm      (Opcode::OC_RESUME,  SIDE_EFFECT,                    1, 1));

    m_put("not",                    new SingleOpcodeCompilerVm      (Opcode::OC_NOT,     NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("type",                   new SingleOpcodeCompilerVm      (Opcode::OC_TYPE,    NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("length",                 new SingleOpcodeCompilerVm      (Opcode::OC_LENGTH,  NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));

    m_put("yield",                  new SingleOpcodeCompilerVm      (Opcode::OC_YIELD,   SIDE_EFFECT,                    0, 0));
    m_put("return",                 new SingleOpcodeCompilerVm      (Opcode::OC_RET,     SIDE_EFFECT    | PUSHES_RESULT, 1, 1));

    m_put("iterator",               new SingleOpcodeCompilerVm      (Opcode::OC_ITER,    NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("iterator?",              new SingleOpcodeCompilerVm      (Opcode::OC_ITERP,   NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("next",                   new SingleOpcodeCompilerVm      (Opcode::OC_NEXT,    SIDE_EFFECT    | PUSHES_RESULT, 1, 1));
    m_put("next?",                  new SingleOpcodeCompilerVm      (Opcode::OC_NEXTP,   NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));

    m_put("not-nil?",               new SingleOpcodeCompilerVm      (Opcode::OC_NOTNILP, NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("atom?",                  new SingleOpcodeCompilerVm      (Opcode::OC_ATOMP,   NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("pair?",                  new SingleOpcodeCompilerVm      (Opcode::OC_PAIRP,   NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("nil?",                   new SingleOpcodeCompilerVm      (Opcode::OC_NILP,    NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("number?",                new SingleOpcodeCompilerVm      (Opcode::OC_NUMP,    NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("integer?",               new SingleOpcodeCompilerVm      (Opcode::OC_INTP,    NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("real?",                  new SingleOpcodeCompilerVm      (Opcode::OC_REALP,   NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("string?",                new SingleOpcodeCompilerVm      (Opcode::OC_STRINGP, NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("symbol?",                new SingleOpcodeCompilerVm      (Opcode::OC_SYMBOLP, NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("assoc?",                 new SingleOpcodeCompilerVm      (Opcode::OC_ASSOCP,  NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("user-type?",             new SingleOpcodeCompilerVm      (Opcode::OC_USERP,   NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("type?",                  new SingleOpcodeCompilerVm      (Opcode::OC_TYPEP,   NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));

    m_put("of-type?",               new SingleOpcodeCompilerVm      (Opcode::OC_OFTYPEP, NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));

    m_put("|",                      new SingleOpcodeCompilerVm      (Opcode::OC_BITOR,   NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put("&",                      new SingleOpcodeCompilerVm      (Opcode::OC_BITAND,  NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put("^",                      new SingleOpcodeCompilerVm      (Opcode::OC_BITXOR,  NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put("~",                      new SingleOpcodeCompilerVm      (Opcode::OC_BITNOT,  NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));

    m_put(">>",                     new SingleOpcodeCompilerVm      (Opcode::OC_SHR,     NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put("<<",                     new SingleOpcodeCompilerVm      (Opcode::OC_SHL,     NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));

    m_put("+",                      new SingleOpcodeCompilerVm      (Opcode::OC_ADD,     NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put("-",                      new SingleOpcodeCompilerVm      (Opcode::OC_SUB,     NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put("*",                      new SingleOpcodeCompilerVm      (Opcode::OC_MUL,     NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));

    m_put("/",                      new SingleOpcodeCompilerVm      (Opcode::OC_DIV,     SIDE_EFFECT    | PUSHES_RESULT, 2, 2)); // has side effect only if division by 0
    m_put("%",                      new SingleOpcodeCompilerVm      (Opcode::OC_MOD,     SIDE_EFFECT    | PUSHES_RESULT, 2, 2)); // has side effect only if division by 0

    m_put(">",                      new SingleOpcodeCompilerVm      (Opcode::OC_GT,      NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put(">=",                     new SingleOpcodeCompilerVm      (Opcode::OC_GTE,     NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put("<",                      new SingleOpcodeCompilerVm      (Opcode::OC_LT,      NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put("<=",                     new SingleOpcodeCompilerVm      (Opcode::OC_LTE,     NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));
    m_put("=",                      new SingleOpcodeCompilerVm      (Opcode::OC_EQL,     NO_SIDE_EFFECT | PUSHES_RESULT, 2, 2));

    m_put("copy",                   new SingleOpcodeCompilerVm      (Opcode::OC_CPLIST,  NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));
    m_put("copy-tree",              new SingleOpcodeCompilerVm      (Opcode::OC_CPTREE,  NO_SIDE_EFFECT | PUSHES_RESULT, 1, 1));

    m_put("list",                   new SingleOpcodeLengthCompilerVm(Opcode::OC_LIST,    NO_SIDE_EFFECT | PUSHES_RESULT, 1, -1));
    m_put("array",                  new SingleOpcodeLengthCompilerVm(Opcode::OC_ARRAY,   NO_SIDE_EFFECT | PUSHES_RESULT, 1, -1));
    m_put("append",                 new SingleOpcodeLengthCompilerVm(Opcode::OC_APPEND,  NO_SIDE_EFFECT | PUSHES_RESULT, 2, -1));

    m_put("call",                   new CallCompilerVm);

    m_put("_invoke",                new InvokeCompilerVm);
    m_put("_evaluate_invoke",       new InvokeExprCompilerVm);

    return m_map;
}