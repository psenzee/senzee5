// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _FRAME_H
#define _FRAME_H

#include <vector>

#include "object.h"

class FunctionHandle;

class Frame
{   
public:    
	
    const unsigned char    *code;
    int                     ip,
                            base;
    Object                **locals;
    size_t                  localcount,
                            argcount;
    FunctionHandle         *function;

    inline Frame() : code(0), ip(0), base(0), locals(0), localcount(0), argcount(0), function(0) {}

    Frame(FunctionHandle *fh, Object **locals, unsigned localcount, unsigned argcount);

    static Frame        *copy(Frame *f);
    static Frame        *deepcopy(Frame *f);

    void                 collect(IMemoryManager *mm)                      { for (unsigned i = 0; i < localcount; i++) mm->collect(locals[i]); }

    Pair                *args();

    inline int           nextbyte()                                       { return (char)code[ip++]; }
    inline unsigned      nextubyte()                                      { return (unsigned char)code[ip++]; }
    inline int           nextshort()                                      { short v = *((short *)(code + ip)); ip += sizeof(short); return v; }
    inline unsigned      nextushort()                                     { unsigned short v = *((unsigned short *)(code + ip)); ip += sizeof(unsigned short); return v; }
    inline int           nextint()                                        { int   v = *((int *)(code + ip)); ip += sizeof(int); return v; }
    inline unsigned      nextuint()                                       { unsigned v = *((unsigned *)(code + ip)); ip += sizeof(unsigned); return v; }
    inline Object       *nextobject()                                     { return (Object *)nextuint(); }

    inline Object       *local(unsigned i)                                { return locals[i]; }

    inline void          setlocal(unsigned i, Object *u)                  { locals[i] = u; }
   
    void                 setlocals(std::vector<Object *> &stack, unsigned count);
};

#endif  // _FRAME_H 