// pool_c - simple memory pooling for strings, Paul S (c) 2014

#ifndef _POOL_C_H
#define _POOL_C_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pool_t
{
    char   *data;
    char   *pointer;
    size_t  size;
    
} pool_t;

typedef struct idx_pool_t
{
    // core part same as pool_t
    char   *data;
    char   *pointer;
    size_t  size;
    
    // index
    char  **index;
    char  **index_pointer;
    size_t  index_size;
    
} idx_pool_t;

pool_t         *pool_create(size_t size);
void           *pool_alloc(pool_t *pool, size_t size);
char           *pool_copy_str(pool_t *pool, const char *str);
void            pool_free(pool_t *pool);
int             pool_owns_pointer(pool_t *pool, const void *p);

size_t          pool_allocated_bytes(pool_t *pool);
size_t          pool_free_bytes(pool_t *pool);
int             pool_has_sufficient_free_bytes(pool_t *pool, size_t size);

idx_pool_t     *idx_pool_create(size_t size, size_t index_size);
void           *idx_pool_alloc(idx_pool_t *pool, size_t size);
char           *idx_pool_copy_str(idx_pool_t *pool, const char *str);
void            idx_pool_free(idx_pool_t *pool);
int             idx_pool_owns_pointer(idx_pool_t *pool, const void *p);

size_t          idx_pool_allocated_bytes(idx_pool_t *pool);
size_t          idx_pool_index_allocated_bytes(idx_pool_t *pool);
size_t          idx_pool_free_bytes(idx_pool_t *pool);
int             idx_pool_has_sufficient_free_bytes(idx_pool_t *pool, size_t size);
size_t          idx_pool_index_free_bytes(idx_pool_t *pool);

size_t          idx_pool_index_count(idx_pool_t *pool);
void           *idx_pool_item_at_index(idx_pool_t *pool, size_t index);

void            idx_pool_string_index_sort(idx_pool_t *pool);
void            idx_pool_consolidate_strings_into(idx_pool_t *from, idx_pool_t *into);
idx_pool_t     *idx_pool_consolidate_strings(idx_pool_t *pool);

int             idx_pool_bsearch_find(idx_pool_t *pool, const char *item);
char           *idx_pool_bsearch_find_return_string(idx_pool_t *pool, const char *item);
    
#ifdef __cplusplus
}
#endif

#endif // _POOL_C_H
