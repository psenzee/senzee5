#include "CorePropertySet.h"

NullPropertySet NullPropertySet::instance;

bool CorePropertySet::SetProperty(const char *key, const char *value)
{
    if (!key) return false;
    mProperties[key] = value;
    return true;
}

const char *CorePropertySet::GetProperty(const char *key) const
{
    if (!key) return 0;
    std::map<String, String>::const_iterator i = mProperties.find(key), e = mProperties.end();
    return i == e ? 0 : (*i).second.c_str();
}