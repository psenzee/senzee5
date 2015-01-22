// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _CONSTANTPOOL_H
#define _CONSTANTPOOL_H

#include <vector>

#include "code_t.h"
#include "numbert.h"
#include "pair.h"
#include "streams.h"
#include "simplemap.h"

class ConstantPool
{
    std::vector<Object *>      m_list;
    SimpleMap                  m_functions;

    enum
	{
        INT       = 0,
        LONG      = 1,
        FLOAT     = 2,
        DOUBLE    = 3,
        STRING    = 4,
        SYMBOL    = 5,
        FUNCTION  = 6,
        PAIR      = 7,
        LIST      = 8,
        NIL       = 9
    };

public:

    ConstantPool(IInStream &s);
    ConstantPool(const ConstantPool &other);

    void                  collect(IMemoryManager *mm);

    int                   count();
    Object               *constant(int index);

  	bool                  isfunction(const char *name);
  	int                   functionid(const char *name);

    void                  read(IInStream &s);

private:

    Object               *m_read(IInStream &s);
    Pair                 *m_read_pair(IInStream &s);
    Pair                 *m_read_list(IInStream &s);
};

#endif  // _CONSTANTPOOL_H