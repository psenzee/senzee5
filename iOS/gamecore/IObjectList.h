#ifndef _IOBJECTLIST_H
#define _IOBJECTLIST_H

class IGameObject;
class ICollidable;

class IObjectList
{
public:
    virtual ~IObjectList() {}

    virtual IGameObject **GetObjects()           = 0;
    virtual int           GetObjectCount() const = 0;
};

class NullObjectList : public IObjectList
{
public:

    IGameObject **GetObjects()           { return 0; }
    int           GetObjectCount() const { return 0; }

    static NullObjectList instance;
};

#endif // _IOBJECTLIST_H
