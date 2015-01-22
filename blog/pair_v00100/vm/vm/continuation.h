// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _CONTINUATION_H
#define _CONTINUATION_H

#include <vector>

#include "context.h"
#include "object.h"
/* $todo, let's make sure i'm doing this right..
class Continuation : public GcObject
{
public:

    Context              *context; // note: if you want to pass continuations among threads, you'll need to other copy relevant context.* data into here instead
	std::vector<Frame *>  frames;
	std::vector<Object *> stack;

    Continuation(Context *context) : context(context)
    {
        copy(context->frames, frames);
        stack = context->stack;
    }
    void restore(Context *c)
    {
        c->stack = stack;
		copy(frames, c->frames);
    }
    Continuation       *cast_continuation()       { return this; }
    const Continuation *cast_continuation() const { return this; }
    const Type         &type()              const { return type_continuation; }

protected:

    inline void copy(const std::vector<Frame *> &from, std::vector<Frame *> &to)
    {
        to = from;
        for (int i = 0, sz = to.size(); i < sz; i++)
            to[i] = Frame::copy(to[i]);
    }
};
*/

#endif  // _CONTINUATION_H

