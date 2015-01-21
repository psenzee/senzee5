// pool_c - simple memory pooling, Paul Senzee (c) 2014

#include "pool_c.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// pool

size_t pool_allocated_bytes(pool_t *pool)
{
    return !pool ? 0 : (pool->pointer - pool->data) * sizeof(char);
}

size_t pool_free_bytes(pool_t *pool)
{
    return !pool ? 0 : (pool->size - pool_allocated_bytes(pool));
}

int pool_has_sufficient_free_bytes(pool_t *pool, size_t size)
{
    return (pool ? (pool->size - (pool->pointer - pool->data)) : 0) >= size;
}

pool_t *pool_create(size_t size)
{
    pool_t *pool = (pool_t *)malloc(sizeof(pool_t));
    memset(pool, 0, sizeof(pool_t));
    pool->size = size;
    pool->data = pool->pointer = (char *)malloc(size);
    return pool;
}

void *pool_alloc(pool_t *pool, size_t size)
{
    if (!size || !pool_has_sufficient_free_bytes(pool, size))
        return NULL;
    char *p = pool->pointer;
    pool->pointer += size;
    return p;
}

char *pool_copy_str(pool_t *pool, const char *str)
{
    if (!str) return NULL;
    size_t sz = strlen(str);
    char *p = (char *)pool_alloc(pool, sz + 1);
    if (p) memcpy(p, str, sz + 1);
    return p;
}

void pool_free(pool_t *pool)
{
    if (pool)
    {
        free(pool->data);
        memset(pool, 0, sizeof(pool_t));
        free(pool);
    }
}

int pool_owns_pointer(pool_t *pool, const void *p)
{
    return !pool ? 0 : (((const char *)p) - pool->data) < pool->size;
}

// indexed pool

size_t idx_pool_allocated_bytes(idx_pool_t *pool)
{
    return !pool ? 0 : (pool->pointer - pool->data) * sizeof(char);
}

size_t idx_pool_index_allocated_bytes(idx_pool_t *pool)
{
    return !pool ? 0 : (pool->index_pointer - pool->index) * sizeof(char *);
}

size_t idx_pool_free_bytes(idx_pool_t *pool)
{
    return !pool ? 0 : (pool->size - idx_pool_allocated_bytes(pool));
}

int idx_pool_has_sufficient_free_bytes(idx_pool_t *pool, size_t size)
{
    if (!pool) return 0;
    size_t free = pool->size - (pool->pointer - pool->data);
    size_t index_free = pool->index_size - ((pool->index_pointer - pool->index) * sizeof(char *));
    return free >= size && index_free >= sizeof(char *);
}

size_t idx_pool_index_free_bytes(idx_pool_t *pool)
{
    return !pool ? 0 : (pool->index_size - idx_pool_index_allocated_bytes(pool));
}

idx_pool_t *idx_pool_create(size_t size, size_t index_size)
{
    idx_pool_t *pool = (idx_pool_t *)malloc(sizeof(idx_pool_t));
    memset(pool, 0, sizeof(idx_pool_t));
    pool->size = size;
    pool->index_size = index_size;
    pool->data = pool->pointer = (char *)malloc(size);
    pool->index = pool->index_pointer = (char **)malloc(index_size);
    return pool;
}

void *idx_pool_alloc(idx_pool_t *pool, size_t size)
{
    if (!size || !idx_pool_has_sufficient_free_bytes(pool, size))
        return NULL;
    char *p = pool->pointer;
    pool->pointer += size;
    *(pool->index_pointer) = p;
    pool->index_pointer++;
    return p;
}

char *idx_pool_copy_str(idx_pool_t *pool, const char *str)
{
    if (!str) return NULL;
    size_t sz = strlen(str);
    char *p = (char *)idx_pool_alloc(pool, sz + 1);
    if (p) memcpy(p, str, sz + 1);
    return p;
}

size_t idx_pool_index_count(idx_pool_t *pool)
{
    return idx_pool_index_allocated_bytes(pool) / sizeof(char *);
}

void *idx_pool_item_at_index(idx_pool_t *pool, size_t index)
{
    return (void *)pool->index[index];
}

void idx_pool_free(idx_pool_t *pool)
{
    if (pool)
    {
        free(pool->data);
        free(pool->index);
        memset(pool, 0, sizeof(idx_pool_t));
        free(pool);
    }
}

int idx_pool_owns_pointer(idx_pool_t *pool, const void *p)
{
    if (!pool) return 0;
    size_t ddp = ((const char *)p) - pool->data;
    size_t dip = (((const char **)p) - (const char **)pool->index) * sizeof(const char *);
    return (ddp < pool->size || dip < pool->index_size);
}

static int qsort_string_compare_ascend(const void *a, const void *b)
{
    const char *p = *(const char **)a, *q = *(const char **)b;
    return strcmp(p, q);
}

void idx_pool_string_index_sort(idx_pool_t *pool)
{
    qsort(pool->index, pool->index_pointer - pool->index, sizeof(const char *), qsort_string_compare_ascend);
}

void idx_pool_consolidate_strings_into(idx_pool_t *from, idx_pool_t *into)
{
    idx_pool_string_index_sort(from);
    size_t count = idx_pool_index_count(from);
    const char *prev = NULL;
    for (size_t i = 0; i < count; i++)
    {
        const char *item = (char *)idx_pool_item_at_index(from, i);
        if (!prev || strcmp(prev, item) != 0)
        {
            idx_pool_copy_str(into, item);
            prev = item;
        }
    }
}

idx_pool_t *idx_pool_consolidate_strings(idx_pool_t *pool)
{
    idx_pool_t *npool = idx_pool_create(pool->size, pool->index_size);
    idx_pool_consolidate_strings_into(pool, npool);
    return npool;
}

int idx_pool_bsearch_find(idx_pool_t *pool, const char *item)
{
    if (!item || !*item)
        return -1;
    const char **find = &item;
    char **found = (char **)bsearch(find, pool->index, pool->index_pointer - pool->index, sizeof(const char *), qsort_string_compare_ascend);
    if (!found || !*found)
        return -1;
    return (int)(found - pool->index);
}

char *idx_pool_bsearch_find_return_string(idx_pool_t *pool, const char *item)
{
    int id = idx_pool_bsearch_find(pool, item);
    if (id == -1)
        return NULL;
    return (char *)idx_pool_item_at_index(pool, (size_t)id);
}
