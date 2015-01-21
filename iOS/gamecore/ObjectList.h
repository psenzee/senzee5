#ifndef _OBJECTLIST_H
#define _OBJECTLIST_H

#include "core/core.h"
#include "fast/Allocator.h"
#include "CoreObjectList.h"
#include "CoreEventList.h"
#include "IGameObject.h"

class IRenderable;
class IPropertySet;
class IEventList;
class ICollidable;

#include <vector>

class ObjectList : public IGameObject, public CoreObjectList
{
public:
    
    CLASS_NEW_DELETE()

    ObjectList(ICollidable *collidable = 0);
    ObjectList(const std::vector<Event *> &events, ICollidable *collidable = 0);

    // implementation of IGameObject
    void               Retain();
    void               Release();

    IGameObject       *Copy();
    bool               IsMutable() { return true; } // $TODO!
        
    void               Update(const GameTime &time);

    IRenderable       &GetRenderable();
    IObjectList       &GetObjectList();
    IPropertySet      &GetProperties();
    IEventList        &GetEventList();

    ICollidable       *GetCollidable();
  
private:
    
    int            mRef;
    ICollidable   *mCollidable;
    CoreEventList  mEvents;
};

#endif // _OBJECTLIST_H
