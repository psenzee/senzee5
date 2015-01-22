// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _ARRAY_T_H
#define _ARRAY_T_H

struct array_t
{  
    void     **first,
             **last;
    unsigned   capacity;
};

void   create(array_t &list, unsigned capacity = 0);
void   destroy(array_t &list);
void   reserve(array_t &list, unsigned capacity);

void   reverse(array_t &list);
void **find(array_t &list, const void *p);
void   append(array_t &list, void *p);

#endif  // _ARRAY_T_H
