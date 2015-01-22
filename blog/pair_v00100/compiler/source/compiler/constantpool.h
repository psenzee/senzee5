// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _CONSTANTPOOL_H
#define _CONSTANTPOOL_H

#include <vector>
#include <map>

class ConstFunction;

#include "codeutil.h"
#include "numbert.h"
#include "strings.h"
#include "streams.h"

class ConstantPool
{
    std::vector<Object *>      m_list;
	std::map<std::string, int> m_index;

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

    ConstantPool();
    ConstantPool(IInStream &s);
    ConstantPool(const ConstantPool &other);    

    static ConstFunction *create_function(const char *name);

    void                  collect(IMemoryManager *mm);

    int                   count();
    Object               *constant(int index);

	bool                  issymbol(const char *name);
	bool                  isfunction(const char *name);

	int                   symbolid(const char *name, bool create_if_undefined = true);
	int                   functionid(const char *name, bool create_if_undefined = true);

	int                   constantid(Object *u, bool create_if_undefined = true);

    void                  write(IOutStream &s) const;
    void                  read(IInStream &s);    

private:

	static std::string    m_idstr(Object *u);
    static Object        *m_read(IInStream &s);
	static const char    *m_typestr(int type);
    static int            m_type(Object *u);

    Pair                 *m_pair(ConstPair *p);
    Pair                 *m_pair(ConstList *list);
    void                  m_write(IOutStream &s, Object *u) const;
};

#endif  // _CONSTANTPOOL_H