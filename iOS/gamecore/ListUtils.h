#ifndef _LISTUTILS_H
#define _LISTUTILS_H

#include "core/core.h"
#include "core/aabox.h"
#include "gametime.h"

class RenderContext;
class IObjectList;

class ListUtils
{
public:

    // IGameObject
    static bool  IsAnyMutable(IObjectList &list);
    static bool  AreAllMutable(IObjectList &list);
    static void  Update(IObjectList &list, const GameTime &time);

    // IPropertySet
    static bool  SetProperty(IObjectList &list, const char *key, const char *value);

    // IRenderable
    static AABox GetBounds(IObjectList &list);
    static bool  IsAnyVisible(IObjectList &list, RenderContext &context);
    static bool  AreAllVisible(IObjectList &list, RenderContext &context);
    static void  Render(IObjectList &list, RenderContext &context);
    static void  RenderImmediate(IObjectList &list, RenderContext &context);
};

#endif // _LISTUTILS_H