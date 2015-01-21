#include "ObjectList.h"
#include "IGameObject.h"
#include "CoreEventList.h"
#include "CorePropertySet.h"
#include "NullRenderable.h"

ObjectList::ObjectList(ICollidable *collidable) : mRef(1), mCollidable(collidable), mEvents()
{
}

ObjectList::ObjectList(const std::vector<Event *> &events, ICollidable *collidable) : mRef(1), mCollidable(collidable), mEvents(events)
{
}

void ObjectList::Retain()
{
    mRef++;
}

void ObjectList::Release()
{
    if (--mRef == 0)
        delete this;
}

IGameObject *ObjectList::Copy()
{
    this->Retain();
    return this;
}

void ObjectList::Update(const GameTime &time)
{
}

IRenderable &ObjectList::GetRenderable()
{
    return NullRenderable::instance;
}

IObjectList &ObjectList::GetObjectList()
{
    return *static_cast<IObjectList *>(this);
}

IPropertySet &ObjectList::GetProperties()
{
    return NullPropertySet::instance;
}

IEventList &ObjectList::GetEventList()
{
    return mEvents;
}

ICollidable *ObjectList::GetCollidable()
{
    return mCollidable;
}