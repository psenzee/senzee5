// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _POINTERLIST_H
#define _POINTERLIST_H

#include "array.h"

template <typename T>
struct PointerList : public array_t
{  
    inline PointerList(unsigned capacity = 0) { create(*this, capacity); }
    inline ~PointerList()                     { destroy(*this); }
    
    inline T      **find(const T *p)          { return (T **)find(*this, p); }
    inline void     append(T *p)              { append(*this, p); }
    inline void     reverse()                 { reverse(*this); }

    inline T       *at(unsigned i)            { return (T *)first[i]; }
    inline void     set(unsigned i, T *p)     { first[i] = p; }
    inline unsigned count() const             { return last - first; }
    inline void     clear()                   { last = first; }
};

#endif  // _POINTERLIST_H
