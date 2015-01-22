// Copyright (c) 2004-2008 Paul D. Senzee
// http://senzee5.com

#ifndef _RETURN_H
#define _RETURN_H

#include <stddef.h> // for intptr_t

class Object;

class Return
{
public:

    typedef void (*fn_t)(Object *value, void *user);

    fn_t  callback;
    void *user;

    inline Return(fn_t callback = 0, void *user = 0) : callback(callback), user(user) {}

    inline void invoke(Object *value) { if (callback) (callback)(value, user); }
};

class Future
{
public:

    inline Future() : callback(m_callback, 0) { callback.user = this; }

    Return callback;

    inline bool    complete() const { return (intptr_t)result != INCOMPLETE; }
    inline Object *value()          { return (intptr_t)result == INCOMPLETE ? 0 : result; }

private:

    Object * volatile result;

    static void m_callback(Object *result, void *user) { if (user) static_cast<Future *>(user)->result = result; }

    static const intptr_t INCOMPLETE = ~0u;
};

#endif  // _RETURN_H