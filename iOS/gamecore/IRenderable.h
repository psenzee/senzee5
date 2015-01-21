#ifndef _IRENDERABLE_H
#define _IRENDERABLE_H

#include "core/core.h"
#include "core/aabox.h"

class RenderContext;

class IRenderable
{
public:
    virtual ~IRenderable() {}

    virtual void   Render(RenderContext &context)          = 0;
    virtual void   RenderImmediate(RenderContext &context) = 0;    
    virtual bool   IsVisible(RenderContext &context)       = 0;
    virtual AABox  GetBounds()                             = 0;
};

#endif // _IRENDERABLE_H
