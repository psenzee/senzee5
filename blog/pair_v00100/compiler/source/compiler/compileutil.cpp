// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include <vector>

#include "codeutil.h"

#include "compileutil.h"
#include "scriptcompiler.h"
#include "ielementcompiler.h"
#include "constantpool.h"
#include "pair.h"
#include "pairiterator.h"

std::string CompileUtil::code(int *values, const char *pattern)
{
    char   ch = 0;
    std::string c;
	while (ch = *pattern)
	{
        switch (ch)
        {
        case '1' : c += (unsigned char)*values;         break;
        case '2' : CodeUtil::append(c, (short)*values); break;
        case '4' : CodeUtil::append(c, (int)*values);   break;
        default:                                        break;
        }
		pattern++;
		values++;
	}
    return c;
}

std::string CompileUtil::concatenate_compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
	std::vector<std::string> stack;
    PairIter it(p), e(0);
    for (; it != e; ++it)
        stack.push_back(program->compile(*it, pool, ci));
	std::string c;
	while (!stack.empty())
	{
		c += stack.back();
		stack.pop_back();
	}
    return c;
}

std::string CompileUtil::concatenate_compile_forward(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    PairIter it(p), e(0);
	std::string c;
    CompileInfo ccf_ci(ci);
    for (; it != e; ++it)
    {
        if (!it.iter->tail && ccf_ci.compile_final_return)
            ccf_ci.compile_return = true;
        c += program->compile(*it, pool, ccf_ci);
    }
	return c;
}

std::string CompileUtil::compile_block(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci)
{
    return concatenate_compile_forward(p, pool, program, ci.compile_return ? compile_final_return(ci) : compile_no_return(ci));
}