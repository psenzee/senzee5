// words_cpp - simple, fast word handling functions, Paul Senzee (c) 2014

#include "mapint_c.h"
#include <map>

extern "C"
{
    
void *mapint_create(void)
{
    return new std::map<int, int>();
}

void mapint_free(void *map)
{
    delete (std::map<int, int> *)map;
}
    
int mapint_contains(void *map, int key)
{
    std::map<int, int> *m = (std::map<int, int> *)map;
    return (*m).find(key) != (*m).end();
}
    
int mapint_value(void *map, int key)
{
    return (*(std::map<int, int> *)map)[key];
}
    
void mapint_set(void *map, int key, int value)
{
    (*(std::map<int, int> *)map)[key] = value;
}
    
int mapint_increment(void *map, int key)
{
    return (*(std::map<int, int> *)map)[key]++;
}
    
size_t mapint_size(void *map)
{
    return ((std::map<int, int> *)map)->size();
}

int mapint_iterate(void *map, mapint_iterate_fn_t fn, void *context)
{
    std::map<int, int> *m = (std::map<int, int> *)map;
    if (!fn || !m)
        return 0;
    for (std::map<int, int>::const_iterator i = (*m).begin(), e = (*m).end(); i != e; ++i)
        if (!fn((*i).first, (*i).second, map, context))
            return 0;
    return 1;
}
    
} // extern "C"