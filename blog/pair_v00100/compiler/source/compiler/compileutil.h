// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _COMPILEUTIL_H
#define _COMPILEUTIL_H

#include <string>

class ScriptCompiler;
class CompileInfo;
class ConstantPool;
class Pair;

class CompileUtil
{
public:

    static std::string code(int *values, const char *pattern);
    static std::string concatenate_compile(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    static std::string concatenate_compile_forward(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);
    static std::string compile_block(Pair *p, ConstantPool *pool, ScriptCompiler *program, const CompileInfo &ci);

private:

	CompileUtil() {}
};

#endif  // _COMPILEUTIL_H