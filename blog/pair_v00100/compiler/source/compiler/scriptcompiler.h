// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _SCRIPTCOMPILER_H
#define _SCRIPTCOMPILER_H

#include "staticevaluator.h"
#include "compilervmtarget.h"

#include <vector>
#include <map>
#include <string>

class Object;
class ICompileTarget;
class ConstantPool;
class Code;
class Pair;
class Number;
class FunctionHandle;
class CompileInfo;
class IElementCompiler;
class LiteralString;

typedef std::map<std::string, Object *> map_t;
typedef std::map<std::string, int>      local_map_t;
typedef std::map<std::string, int>      global_map_t;

class ScriptCompiler
{
public:

    ScriptCompiler(Pair *p, bool allow_undef, ICompileTarget *target = CompilerVmTarget::INSTANCE);

    bool          allow_undefined() const { return m_allow_undefined; }
    std::string   compile(Object *u, ConstantPool *pool, const CompileInfo &ci);

    int           func_index(Object *u, ConstantPool *pool, bool create_new = true);
    int           const_index(Object *u, ConstantPool *pool, bool create_new = true);
    int           local_index(Object *u, bool create_new = true);
    int           global_index(Object *u, bool create_new = true);

    bool          is_defined_local(const std::string &name) const;
    bool          is_defined_global(const std::string &name) const;

    bool          has_side_effect(Object *u) const;

    void          push_local_scope();
    void          pop_local_scope();

    local_map_t  &locals();
    global_map_t &globals();

    void          add_function(int id, int localsize, int stacksize, const std::string &code);
    void          add_function(FunctionHandle *fh);
    Code         *compile_code();

private:

    Pair                                     *m_root;
	std::vector<local_map_t>                  m_locals;
	global_map_t                              m_globals;
	std::map<std::string, IElementCompiler *> m_compilers;
    std::vector<FunctionHandle *>             m_functions;
    ICompileTarget                           *m_target;
    bool                                      m_allow_undefined;

    std::string m_compile_pair(Pair *p, ConstantPool *pool, const CompileInfo &ci);
    std::string m_compile_object(Object *u, ConstantPool *pool, const CompileInfo &ci);
    std::string m_compile_string_literal(Object *s, ConstantPool *pool, const CompileInfo &ci);
    std::string m_compile_numeric_literal(Object *n, ConstantPool *pool, const CompileInfo &ci);
    std::string m_compile_local_reference(Object *u, const CompileInfo &ci);
    std::string m_compile_global_reference(Object *u, const CompileInfo &ci);

    void        m_arity(IElementCompiler *ec, int arg_count, const char *fnname, bool function);
    std::string m_format(const char *format, const char *fnname, bool function);
};

#endif  // _SCRIPTCOMPILER_H 