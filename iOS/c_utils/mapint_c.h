// a C-only interface for a std::map<int, int>-like class, Paul Senzee (c) 2014

#ifndef _MAPINT_C_H
#define _MAPINT_C_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
    
// return 0 to stop iterating
typedef int (*mapint_iterate_fn_t)(int key, int value, void *map, void *context);
    
void  *mapint_create(void);
void   mapint_free(void *map);
int    mapint_contains(void *map, int key);
int    mapint_value(void *map, int key);
void   mapint_set(void *map, int key, int value);
int    mapint_increment(void *map, int key);
size_t mapint_size(void *map);
int    mapint_iterate(void *map, mapint_iterate_fn_t fn, void *context);
    
#ifdef __cplusplus
}
#endif

#endif // _MAPINT_C_H
