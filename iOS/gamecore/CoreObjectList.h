#ifndef _COREOBJECTLIST_2349820_H
#define _COREOBJECTLIST_2349820_H

#include "IObjectList.h"
#include "fast/Allocator.h"

#include <vector>

class CoreObjectList : public IObjectList
{
public:
    
    CLASS_NEW_DELETE()

    CoreObjectList();
    CoreObjectList(std::vector<IGameObject *> &objects);
    ~CoreObjectList();

    void               Add(IGameObject *o);
    IGameObject       *GetAt(int i);
    const IGameObject *GetAt(int i) const;
    void               Clear();

    // implementation of IObjectList
    int                GetObjectCount() const;
    IGameObject      **GetObjects();

private:
    
    std::vector<IGameObject *> mObjects;
};

#endif // _COREOBJECTLIST_2349820_H