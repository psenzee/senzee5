#ifndef _IGAMEOBJECT_H
#define _IGAMEOBJECT_H

#include "core/core.h"
#include "gametime.h"

class IRenderable;
class IObjectList;
class IPropertySet;
class IEventList;
class ICollidable;

class IGameObject
{
public:
    
    virtual ~IGameObject() {}

    virtual void              Retain()                                    = 0;
    virtual void              Release()                                   = 0;

    virtual IGameObject      *Copy()                                      = 0;
    virtual bool              IsMutable()                                 = 0;
        
    virtual void              Update(const GameTime &time)                = 0;

    virtual IRenderable      &GetRenderable()                             = 0;
    virtual IObjectList      &GetObjectList()                             = 0;
    virtual IPropertySet     &GetProperties()                             = 0;
    virtual IEventList       &GetEventList()                              = 0;

    virtual ICollidable      *GetCollidable()                             = 0;
};

#endif // _IGAMEOBJECT_H
