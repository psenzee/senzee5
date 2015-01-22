// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "frame.h"
#include "functionhandle.h"
#include "pairutil.h"

Frame::Frame(FunctionHandle *fh, Object **locals, unsigned localcount, unsigned argcount) : 
    code((const unsigned char *)fh->code.data()), ip(0), base(0), locals(locals), localcount(localcount), argcount(argcount), function(fh)
{
    memset(locals, 0, sizeof(Object *) * localcount);
}

Frame *Frame::copy(Frame *f)
{
//  return new Frame(f->code, f->ip, f->base, f->locals, f->localcount, f->function);
    return 0; // $todo for now..
}

Frame *Frame::deepcopy(Frame *f)
{
    /*
    Frame *cf = copy(f);
    for (int i = 0, sz = f->localcount; i < sz; i++)
        (cf->locals)[i] = PairUtil::copy((f->locals)[i]);
    return cf;
    */
    return 0;
}

Pair *Frame::args()
{
    return PairUtil::list(locals, argcount);
}

void Frame::setlocals(std::vector<Object *> &stack, unsigned count) // $todo needs to be optimized
{
    base = stack.size();
    for (unsigned i = 0; i < count; i++)
    {
        locals[i] = stack.back();
        stack.pop_back();
    }
}