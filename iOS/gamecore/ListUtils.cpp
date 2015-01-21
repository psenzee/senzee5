#include "ListUtils.h"
#include "IGameObject.h"
#include "IObjectList.h"
#include "IRenderable.h"
#include "IPropertySet.h"
#include "IEventList.h"

AABox ListUtils::GetBounds(IObjectList &list)
{
    AABox box;
    if (!list.GetObjectCount())
        return box;
    for (IGameObject **i = list.GetObjects(), **e = i + list.GetObjectCount(); i < e; ++i)
        box.insert((*i)->GetRenderable().GetBounds());
    return box;
}

bool ListUtils::SetProperty(IObjectList &list, const char *key, const char *value)
{
    bool result = false;
    if (!list.GetObjectCount())
        return result;
    for (IGameObject **i = list.GetObjects(), **e = i + list.GetObjectCount(); i < e; ++i)
        result |= (*i)->GetProperties().SetProperty(key, value);
    return result;
}

bool ListUtils::IsAnyVisible(IObjectList &list, RenderContext &context)
{
    if (!list.GetObjectCount())
        return false;
    for (IGameObject **i = list.GetObjects(), **e = i + list.GetObjectCount(); i < e; ++i)
        if ((*i)->GetRenderable().IsVisible(context))
            return true;
    return false;
}

bool ListUtils::AreAllVisible(IObjectList &list, RenderContext &context)
{
    if (!list.GetObjectCount())
        return false;
    for (IGameObject **i = list.GetObjects(), **e = i + list.GetObjectCount(); i < e; ++i)
        if (!(*i)->GetRenderable().IsVisible(context))
            return false;
    return true;
}

bool ListUtils::IsAnyMutable(IObjectList &list)
{
    if (!list.GetObjectCount())
        return false;
    for (IGameObject **i = list.GetObjects(), **e = i + list.GetObjectCount(); i < e; ++i)
        if ((*i)->IsMutable())
            return true;
    return false;
}

bool ListUtils::AreAllMutable(IObjectList &list)
{
    if (!list.GetObjectCount())
        return false;
    for (IGameObject **i = list.GetObjects(), **e = i + list.GetObjectCount(); i < e; ++i)
        if (!(*i)->IsMutable())
            return false;
    return true;
}

void ListUtils::Update(IObjectList &list, const GameTime &time)
{
    if (!list.GetObjectCount())
        return;
    for (IGameObject **i = list.GetObjects(), **e = i + list.GetObjectCount(); i < e; ++i)
        (*i)->Update(time);
}

void ListUtils::Render(IObjectList &list, RenderContext &context)
{
    if (!list.GetObjectCount())
        return;
    for (IGameObject **i = list.GetObjects(), **e = i + list.GetObjectCount(); i < e; ++i)
        (*i)->GetRenderable().Render(context);
}

void ListUtils::RenderImmediate(IObjectList &list, RenderContext &context)
{
    if (!list.GetObjectCount())
        return;
    for (IGameObject **i = list.GetObjects(), **e = i + list.GetObjectCount(); i < e; ++i)
        (*i)->GetRenderable().RenderImmediate(context);
}