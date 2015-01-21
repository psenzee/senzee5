#ifndef _FIXEDLIST_H
#define _FIXEDLIST_H

#include <string.h>
#include <stdio.h>

template <typename T, int MaxItems>
class FixedList
{
public:

    FixedList();

    bool            Set(int id, const T &value);
    inline int      Add(const T &value);
    bool            Remove(int id);
    inline const T &GetAt(int id)               { return mItems[id].mValue; }
    inline int      GetNext(int id) const       { return GetNextActive(id); }
    void            Clear();
    inline int      Count() const               { return GetActiveCount(); }
    int             FreeCount() const           { return GetFreeCount(); }

    // debug & test
    void            PrintActiveList() const;
    void            PrintFreeList() const;

    inline int      GetNextFree(int id)   const { return id == -1 ? mFirstFree   : mItems[id].mNextFree; }

private:

    void            ClearAll();
    void            ClearActive();

    inline int      GetNextActive(int id) const { return id == -1 ? mFirstActive : mItems[id].mNextActive; }
    inline int      GetPrevActive(int id) const { return id == -1 ? mLastActive  : mItems[id].mPrevActive; }
    inline int      GetPrevFree(int id)   const { return id == -1 ? mLastFree    : mItems[id].mPrevFree; }
    
    int             GetActiveCount()      const;
    int             GetFreeCount()        const;

    struct Item
    {
        T        mValue;
        short    mNextFree,
                 mPrevFree,
                 mNextActive,
                 mPrevActive;
    };
    
    enum { MAX_ITEMS = MaxItems };

    Item  mItems[MAX_ITEMS];
    short mFirstFree,
          mLastFree,
          mFirstActive,
          mLastActive;
};

template <typename T, int MaxItems>
void FixedList<T, MaxItems>::Clear()
{
    int count = GetActiveCount();
    // if less than 1/4 are active, probably faster to clear them only
    if (count < MAX_ITEMS / 4) ClearActive();
    else                       ClearAll();
}

template <typename T, int MaxItems>
void FixedList<T, MaxItems>::ClearActive()
{
    while (mFirstActive != -1)
        Remove(mFirstActive);
}

template <typename T, int MaxItems>
void FixedList<T, MaxItems>::ClearAll()
{
    memset(mItems, 0, sizeof(mItems));
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        mItems[i].mNextActive = mItems[i].mPrevActive = -1;
        mItems[i].mNextFree = static_cast<short>(i + 1);
        mItems[i].mPrevFree = static_cast<short>(i - 1);
    }
    mItems[0].mPrevFree = -1;
    mItems[MAX_ITEMS - 1].mNextFree = -1;
    mFirstFree = 0;
    mLastFree = MAX_ITEMS - 1;
    mFirstActive = -1;
    mLastActive = -1;
}

template <typename T, int MaxItems>
FixedList<T, MaxItems>::FixedList()
{
    ClearAll();
}

template <typename T, int MaxItems>
bool FixedList<T, MaxItems>::Set(int id, const T &value)
{	
    Item &item = mItems[id];
    if (item.mValue)
		return false;

    item.mValue = value;

    // remove from free list
    if (item.mNextFree == -1) mLastFree  = item.mPrevFree;
    else                      mItems[item.mNextFree].mPrevFree = item.mPrevFree;
    if (item.mPrevFree == -1) mFirstFree = item.mNextFree;
    else                      mItems[item.mPrevFree].mNextFree = item.mNextFree;
    item.mNextFree = -1;
    item.mPrevFree = -1;

    // add to active list
    item.mNextActive = mFirstActive;
    item.mPrevActive = (mFirstActive == -1) ? -1 : mItems[mFirstActive].mPrevActive; // should be -1
    if (mFirstActive != -1) mItems[mFirstActive].mPrevActive = id;
    mFirstActive = id;

    return true;
}

template <typename T, int MaxItems>
inline int FixedList<T, MaxItems>::Add(const T &value)
{
    int id = mFirstFree;
    return !Set(id, value) ? -1 : id;
}

template <typename T, int MaxItems>
bool FixedList<T, MaxItems>::Remove(int id)
{
    Item &item = mItems[id];
    if (!item.mValue)
        return false;

    item.mValue = 0;

    // add to free list
    item.mNextFree = mFirstFree;
    item.mPrevFree = (mFirstFree == -1) ? -1 : mItems[mFirstFree].mPrevFree; // should be -1
    if (mFirstFree != -1) mItems[mFirstFree].mPrevFree = id;
    mFirstFree = id;

    // remove from active list
    if (item.mNextActive == -1) mLastActive  = item.mPrevActive;
    else                        mItems[item.mNextActive].mPrevActive = item.mPrevActive;
    if (item.mPrevActive == -1) mFirstActive = item.mNextActive;
    else                        mItems[item.mPrevActive].mNextActive = item.mNextActive;
    item.mNextActive = -1;
    item.mPrevActive = -1;

    return true;
}

template <typename T, int MaxItems>
int FixedList<T, MaxItems>::GetActiveCount() const
{
    int id = GetNextActive(-1), count = 0;
    while (id != -1)
    {
        id = GetNextActive(id);
        count++;
    }
    return count;
}

template <typename T, int MaxItems>
int FixedList<T, MaxItems>::GetFreeCount() const
{
    int id = GetNextFree(-1), count = 0;
    while (id != -1)
    {
        id = GetNextFree(id);
        count++;
    }
    return count;
}

template <typename T, int MaxItems>
void FixedList<T, MaxItems>::PrintActiveList() const
{
    printf("Active: ");
    int id = GetNextActive(-1);
    while (id != -1)
    {
        printf("%d ", id);
        id = GetNextActive(id);
    }
    printf("\n");
}

template <typename T, int MaxItems>
void FixedList<T, MaxItems>::PrintFreeList() const
{
    printf("Free: ");
    int id = GetNextFree(-1);
    while (id != -1)
    {
        printf("%d ", id);
        id = GetNextFree(id);
    }
    printf("\n");
}

#endif // _FIXEDLIST_H