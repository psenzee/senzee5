// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#include "gc.h"

// Garbage Collection

#include <vector>
#include <algorithm>
#include <functional>

#include <stdio.h>
#include <assert.h>

unsigned ICollectable::created    = 0,
         ICollectable::destroyed  = 0;         

class SimpleGarbageCollector : public IMemoryManager
{
public:

    void registerobject(const ICollectable *obj);
    void collect(const ICollectable *obj);
    void clean();

private:

    std::vector<const ICollectable *> m_list;

    void registerobj(const ICollectable *obj);
    void mark(const ICollectable *obj);
    void release(const ICollectable *obj);
    void unmark();
    void sweep();
};

class Marker : public ICollectableVisitor
{
public:

    bool visit(const ICollectable *obj)
    {
        if (obj && !obj->marked())
        {
            obj->mark();
            return true;
        }
        return false; // already marked..
    }
};

void SimpleGarbageCollector::registerobject(const ICollectable *obj)
{ 
    registerobj(obj);
}

void SimpleGarbageCollector::collect(const ICollectable *obj)
{
    if (obj) mark(obj);
}

void SimpleGarbageCollector::clean()
{ 
    unsigned destroyed = ICollectable::destroyed;
    sweep();
    unmark();
}

void SimpleGarbageCollector::registerobj(const ICollectable *obj)
{
    if (obj) m_list.push_back(obj);
}

void SimpleGarbageCollector::mark(const ICollectable *obj)
{
    if (obj)
    {
        Marker marker;
        obj->apply(&marker);
    }    
}

void SimpleGarbageCollector::release(const ICollectable *obj)
{
    if (obj) delete obj;
}

void SimpleGarbageCollector::unmark()
{
    for (std::vector<const ICollectable *>::iterator i = m_list.begin(), e = m_list.end(); i != e; ++i)
        if (*i) (*i)->unmark();
}

void SimpleGarbageCollector::sweep()
{
    // Assumptions: only one instance of any pointer is in this list
    size_t count = m_list.size();
    for (std::vector<const ICollectable *>::iterator i = m_list.begin(), e = m_list.end(); i != e; ++i)
    {
        const ICollectable *obj = *i;
        if (obj && !obj->marked())
        {
            release(obj);
            *i = 0;
        }
    }
    m_list.erase(std::remove(m_list.begin(), m_list.end(), static_cast<const ICollectable *>(0)), m_list.end());
}

IMemoryManager *MemoryManager::instance = 0;

IMemoryManager &MemoryManager::manager()
{
    if (!instance)
        instance = new SimpleGarbageCollector;
    return *instance;
}