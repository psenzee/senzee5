// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _CONTEXT_H
#define _CONTEXT_H

#include "object.h"
#include "function.h"
#include "iresolvecontext.h"

#include <vector>
#include <map>

#include <assert.h>

class Code;
class Frame;
class Package;

#include "frame.h"

typedef void (*complete_function_t)(Object *value, void *user);

class Context : public IResolveContext
{
public:

    enum { MAX_LOCALS = 1024, MAX_FUNCTIONS = 1024, MAX_GLOBALS = 1024 };

    static Object              **globals;

    std::vector<Object *>        stack;
    std::vector<Frame>           frames;
    Object                     **local_pool,
                               **local_pool_at;
	Function                   **fnnative; // only used until link time
    Code                        *program;

    Context(Code *program, Package &package);
    Context(Context &context);
    ~Context();

    int             functionid(const char *name);
    void            clear();

    std::string     currentfunctionname();
    std::string     functionname(int id);
    std::string     position();

    void            collect(IMemoryManager *mm);

    inline Frame   *frame()                            { return (Frame *)&frames.back(); }
	inline void     call(Function *fn, int count)      { fn->evaluate(this, stack, count); }
    inline Object  *returnvalue()                      { return stack.empty() ? 0 : stack.back(); }

    Object        **allocatelocals(unsigned count);
    void            freelocals(unsigned count);

    void            apply(Function *fn, Object *list);
    Function       *function(int fnindex);
    Object         *constant(int index);
    void            doreturn();
    void            link(const char *name, Function *fn);
	void            link(Package &package);

    static Object **global(unsigned i);
};

#endif  // _CONTEXT_H