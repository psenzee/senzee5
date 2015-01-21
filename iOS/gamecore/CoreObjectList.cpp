#include "CoreObjectList.h"
#include "IGameObject.h"

NullObjectList NullObjectList::instance;

CoreObjectList::CoreObjectList()
{
}

CoreObjectList::~CoreObjectList()
{
    Clear();
}

CoreObjectList::CoreObjectList(std::vector<IGameObject *> &objects) : mObjects(objects)
{
    for (std::vector<IGameObject *>::iterator i = objects.begin(), e = objects.end(); i != e; ++i)
        if (*i) (*i)->Retain();
}

void CoreObjectList::Clear()
{
    for (std::vector<IGameObject *>::iterator i = mObjects.begin(), e = mObjects.end(); i != e; ++i)
        if (*i) (*i)->Release();
    mObjects.clear();        
}

void CoreObjectList::Add(IGameObject *object)
{
    if (object)
    {
        object->Retain();
        mObjects.push_back(object);
    }
}
int CoreObjectList::GetObjectCount() const
{
    return (int)mObjects.size();
}

IGameObject *CoreObjectList::GetAt(int i)
{ 
    return mObjects.empty() ? 0 : mObjects[i];
}

const IGameObject *CoreObjectList::GetAt(int i) const
{
    return mObjects.empty() ? 0 : mObjects[i];
}

IGameObject **CoreObjectList::GetObjects()
{
    return &mObjects[0];
}