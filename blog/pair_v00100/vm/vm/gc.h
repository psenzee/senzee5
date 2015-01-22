// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _GC_H
#define _GC_H

// Garbage Collection

class ICollectable;

class ICollectableVisitor
{
public:
    virtual ~ICollectableVisitor() {}

    virtual bool visit(const ICollectable *obj) = 0;
};

class ICollectable
{
public:    

    virtual ~ICollectable() {}

    virtual void     apply(ICollectableVisitor *v)  const = 0; // if Visit() returns false, don't process children

    virtual bool     marked()                       const = 0;
    virtual void     mark()                         const = 0;
    virtual void     unmark()                       const = 0;

    static unsigned created,
                    destroyed;
};

template <typename T>
class Tracked : public T
{
public:
    inline Tracked()                              { ICollectable::created++;   }
    ~Tracked()                                    { ICollectable::destroyed++; }
};

template <typename T>
class Collectable : public T
{
public:

    inline Collectable() : m_mark(false)          { ICollectable::created++; }
    ~Collectable()                                { ICollectable::destroyed++; }

    void     apply(ICollectableVisitor *v)  const { if (v) v->visit(this); }

    bool     marked()                       const { return m_mark; }
    void     mark()                         const { m_mark = true; }
    void     unmark()                       const { m_mark = false; }

private:

    mutable bool m_mark;
};

template <typename T>
class Plain : public T
{
public:

    void     apply(ICollectableVisitor *v)  const {}

    bool     marked()                       const { return true; }
    void     mark()                         const {}
    void     unmark()                       const {}

private:
};

class IMemoryManager
{
public:

    virtual ~IMemoryManager() {}

    virtual void registerobject(const ICollectable *obj)    = 0;
    virtual void collect(const ICollectable *obj)           = 0; // call collect for all items on the stack, in registers, etc..
    virtual void clean()                                    = 0; // then call clean
};

class MemoryManager
{
public:
    static IMemoryManager &manager();
private:
    static IMemoryManager *instance;
    MemoryManager();
};

template <typename T> inline T *_gc_new(T *p) { MemoryManager::manager().registerobject(p); return p; }

#endif // _GC_H