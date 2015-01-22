// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "functionhandle.h"
#include "constantpool.h"
#include "iresolvecontext.h"

#include "context.h"
#include "frame.h"
#include "opcode.h"
#include "config.h"

#include "read_type.h"

void FunctionHandle::evaluate(Context *context, std::vector<Object *> &stack, unsigned count)
{
    context->frames.push_back(Frame(this, context->allocatelocals(localsize), localsize, count));
    context->frames.back().setlocals(stack, count);
}

extern unsigned char *expand(const unsigned char *code, unsigned sz, unsigned *outsize);

bool do_expand = true;

void FunctionHandle::read(IInStream &s)
{
    id = read_type<short>(s);
    stacksize = read_type<unsigned short>(s);
    localsize = read_type<unsigned short>(s);
    code = read_prefix_str<unsigned short, code_t::value_type>(s);
    if (do_expand)
    {
        // ok, expand out all bytecodes that take an immediate argument
        unsigned outsize = 0;
        unsigned char *expanded = expand((const unsigned char *)code.data(), (unsigned)code.size(), &outsize);
        code.assign((const char *)expanded, outsize);
    }
}

void FunctionHandle::apply(ICollectableVisitor *v) const
{
    if (v->visit(this))
        for (std::vector<Object *>::const_iterator i = m_constants.begin(), e = m_constants.end(); i != e; ++i)
            if (*i) (*i)->apply(v);
}

void FunctionHandle::resolve(IResolveContext *context)
{
    size_t size = code.size();
    unsigned char *data = new unsigned char [size];
    memcpy(data, code.data(), size);
    int oc = 0, id = 0;
    Object *c = 0, **g = 0;
    Function *func = 0;
    for (size_t p = 0; p < size;)
    {
        oc = data[p];
        switch (oc)
        {
        case Opcode::OC_FUNC:
            memcpy(&id, data + p + 1, sizeof(intptr_t));
            func = context->function(id);
            m_constants.push_back(func);
            memcpy(data + p + 1, &func, sizeof(intptr_t));
            data[p] = Opcode::OC_LINK_FUNC;
            break;
        case Opcode::OC_CONST:
            memcpy(&id, data + p + 1, sizeof(intptr_t));
            c = context->constant(id);
            m_constants.push_back(c);
            memcpy(data + p + 1, &c, sizeof(intptr_t));
            data[p] = Opcode::OC_LINK_CONST;
            break;
        case Opcode::OC_GLOBAL4:
            memcpy(&id, data + p + 1, sizeof(intptr_t));
            g = Context::global(id);
            memcpy(data + p + 1, &g, sizeof(intptr_t));
            data[p] = Opcode::OC_LINK_GLOBAL;
            break;
        case Opcode::OC_SETG4:
            memcpy(&id, data + p + 1, sizeof(intptr_t));
            g = Context::global(id);
            memcpy(data + p + 1, &g, sizeof(intptr_t));
            data[p] = Opcode::OC_LINK_SETG;
            break;
        }
        p += Opcode::width(oc);
    }
    code.clear();
    code.append((const code_t::value_type *)data, size);
    delete [] data;
}