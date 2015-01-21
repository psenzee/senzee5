#ifndef _FIXEDSTRING_H
#define _FIXEDSTRING_H

#include <string.h>
#include "core_assert.h"

template <unsigned short SIZE>
class FixedString
{
public:
    enum { MAX_SIZE = SIZE - 1 };

    inline FixedString()                               { assign(0); }
    inline FixedString(const char *s)                  { assign(s); }
    inline FixedString(const char *s, size_t length)   { assign(s, length); }
    inline FixedString(const FixedString<SIZE> &other) { assign(other.data(), other.size()); }

    inline FixedString &operator=(const FixedString<SIZE> &other)
    {
        if (this != &other)
            assign(other.data(), other.size());
        return *this;
    }

    inline bool operator<(const FixedString<SIZE> &other) const
    {
        return strcmp(mString, other.mString) < 0;
    }

    inline void        resize(size_t count, char c)
    { 
        if (count > MAX_SIZE) // make sure it fits
            count = MAX_SIZE;
        mSize = static_cast<unsigned short>(count);
        memset(mString, c, count);
        mString[count] = 0;
    }

    inline size_t      size()  const { return static_cast<size_t>(mSize); }
    inline bool        empty() const { return !mSize; }
    inline void        clear()       { assign(0); }
    inline const char *c_str() const { return mString; }
    inline const char *data()  const { return mString; }
    inline char       *data()        { return mString; }

    inline void assign(const char *s)
    {
        mSize = 0;
        mString[0] = '\0';
        if (!s || !*s)
            memset(mString, 0, SIZE);
        else
        {
            size_t ssz = strlen(s);
            if (ssz > MAX_SIZE) // make sure it fits
                ssz = MAX_SIZE;
            strncpy(mString, s, ssz);
            mString[ssz] = '\0';
            mSize = static_cast<unsigned short>(ssz);
        }
    }

    inline void assign(const char *s, size_t length)
    {
        if (length > MAX_SIZE) // make sure it fits
            length = MAX_SIZE;
        memcpy(mString, s, length);
        mSize = static_cast<unsigned short>(length);
        mString[length] = '\0';
    }

private:

    unsigned short mSize;
    char           mString[SIZE];
};

#endif // _FIXEDSTRING_H