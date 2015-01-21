#ifndef _COREPROPERTYSET_H
#define _COREPROPERTYSET_H

#include "core/core.h"
#include "fast/Allocator.h"
#include "IPropertySet.h"

#include <map>

class NullPropertySet : public IPropertySet
{
public:
    
    CLASS_NEW_DELETE()

    bool        SetProperty(const char *key, const char *value) { return false; }
    const char *GetProperty(const char *key) const              { return 0; }

    static NullPropertySet instance;
};

class CorePropertySet : public IPropertySet
{
public:
    
    CLASS_NEW_DELETE()    

    bool        SetProperty(const char *key, const char *value);
    const char *GetProperty(const char *key) const;

private:

    std::map<String, String> mProperties;
};


#endif // _COREPROPERTYSET_H
