#include "properties_c.h"
#include "events_c.h"
#include "strs_c.h"

#include <stdio.h>
#include <string>
#include <map>
#include <pthread.h>

std::map<std::string, Variant> PROPERTIES;
pthread_mutex_t PROPERTIES_MUTEX = PTHREAD_MUTEX_INITIALIZER;

extern "C"
{

void Property_Set(const char *key, Variant value)
{
    if (strs_ends(key, "LIGHT_TABLE_NUMBER", 1))
        printf("setting '%s' to '%s'\n", key, Variant_StringDescription(&value));
    pthread_mutex_lock(&PROPERTIES_MUTEX);
    PROPERTIES[key] = value;
    pthread_mutex_unlock(&PROPERTIES_MUTEX);
    Events_Fire("PROPERTIES", key, key);
}

Variant Property_Get(const char *key)
{
    pthread_mutex_lock(&PROPERTIES_MUTEX);
    static std::string ks; // reduce unnecessary allocations
    ks = key;
    std::map<std::string, Variant>::const_iterator found = PROPERTIES.find(ks);
    Variant fv = Variant_Null();
    if (found != PROPERTIES.end())
        fv = (*found).second;
    if (strs_ends(key, "LIGHT_TABLE_NUMBER", 1))
        printf("getting '%s' which is '%s'\n", key, Variant_StringDescription(&fv));
    pthread_mutex_unlock(&PROPERTIES_MUTEX);
    return fv;
}

void Property_RegisterSetEvent(const char *key, EventHandler *eh)
{
    Events_Register("PROPERTIES", key, eh);
}

void Property_UnregisterSetEvent(const char *key, EventHandler *eh)
{
    Events_Unregister("PROPERTIES", key, eh);
}

}
