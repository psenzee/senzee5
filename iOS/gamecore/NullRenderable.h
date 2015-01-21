#ifndef _NULLRENDERABLE_H
#define _NULLRENDERABLE_H

#include "IRenderable.h"
#include "fast/Allocator.h"

class NullRenderable : public IRenderable
{
public:
    
    CLASS_NEW_DELETE()

    void          Render(RenderContext &context)                  {}
    void          RenderImmediate(RenderContext &context)         {}
    bool          IsVisible(RenderContext &context)               { return false; }
    AABox         GetBounds()                                     { return AABox(); }

    static NullRenderable instance;
};

#endif // _NULLRENDERABLE_H
