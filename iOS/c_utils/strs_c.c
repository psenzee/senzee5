// Pure C String and Parsing Utilities, Paul S (c) 2000-2015

#include "strs_c.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>

#define _min_(A, B) ((A) <= (B) ? (A) : (B))

#ifndef WIN32
int memicmp(const void *s1, const void *s2, size_t n);

int memicmp(const void *s1, const void *s2, size_t n)
{
    const char *c1 = (const char *)s1,
               *c2 = (const char *)s2;
    int d = 0;
    for (const char *ce = c1 + n; c1 < ce; c1++, c2++)
    {
        d = tolower(*c1) - tolower(*c2);
        if (d != 0)
            return d;
    }
    return 0;
}
#endif

#define SMALL_TMP_BUFFER_SIZE (64 * 1024)

static char  SMALL_TMP_BUFFER[SMALL_TMP_BUFFER_SIZE];
static _bool TMP_BUFFER_IN_USE = 0;

char *strs_tmp_buffer(size_t size)
{
    assert(!TMP_BUFFER_IN_USE);
    char *buffer = (size < SMALL_TMP_BUFFER_SIZE - 1) ? SMALL_TMP_BUFFER : (char *)malloc(size);
    memset(buffer, 0, size);
    TMP_BUFFER_IN_USE = _true;
    return buffer;
}

void strs_free_tmp_buffer(const char *buffer) // c++ allows deletion of const pointers, so we will too
{
    assert(TMP_BUFFER_IN_USE);
    if (buffer != SMALL_TMP_BUFFER)
        free((void *)buffer);
    TMP_BUFFER_IN_USE = _false; // allow to be reused
}

static void *malloc_and_clear(size_t size, _bool clear)
{
    void *data = malloc(size);
    if (clear)
        memset(data, 0, size);
    return data;
}

static char *new_chars(size_t count, _bool clear)
{
    return malloc_and_clear(count, clear);
}

static char **new_char_ptrs(size_t count, _bool clear)
{
    return (char **)malloc_and_clear(count * sizeof(char *), clear);
}

char *strs_strdup(const char *s)
{
    if (!s) return 0;
    size_t len = strlen(s);
    char *c = new_chars(len + 1, _true);
    strncpy(c, s, len);
    return c;
}

int strs_strlistcount(const char **p)
{
    if (!p || !*p) return 0;
    int count = 0;
    for (const char **plist = p; *plist; count++, plist++) {} // count items..
    return count;
}

char **strs_strlistdup(const char **p)
{
    int count = strs_strlistcount(p);
    char **nlist = new_char_ptrs(count + 1, _true);
    for (int i = 0; i < count; i++)
      nlist[i] = strs_strdup(p[i]);
    return nlist;
}

void strs_destroy_strlist(char **p)
{
    if (p)
    {
        for (char **plist = p; *plist; plist++)
            free(*plist);
        free(p);
    }
}

void strs_skip_space(const char **s)
{
    while (**s && isspace(**s)) (*s)++;
}

void strs_skip_space_not_newline(const char **s)
{
    while (**s && isspace(**s) && **s != '\n' && **s != '\r') (*s)++;
}

_bool strs_is(const char **s, const char *match, _bool ignorecase)
{
    const char *p = *s;
    strs_skip_space(&p);
    if (!strs_starts(p, match, ignorecase))
        return _false;
    p += strlen(match);
    *s = p;
    return _true;
}

_bool strs_is_and_followed_by_space(const char **s, const char *match, _bool ignorecase)
{
    const char *p = *s;
    strs_skip_space(&p);
    if (!strs_starts(p, match, ignorecase))
        return _false;
    p += strlen(match);
    if (!isspace(*p) && *p != 0) // make sure that it's the whole word..
        return _false;
    *s = p;
    return _true;
}

_bool strs_is_and_followed_by_any_of(const char **s, const char *match, const char *follow, _bool ignorecase)
{
    const char *p = *s;
    strs_skip_space(&p);
    if (!strs_starts(p, match, ignorecase))
        return _false;
    p += strlen(match);
    if (!strchr(follow, *p) && *p != 0) // make sure that it's the whole word..
        return _false;
    *s = p;
    return _true;
}

_bool strs_is_and_not_followed_by_any_of(const char **s, const char *match, const char *follow, _bool ignorecase)
{
    const char *p = *s;
    strs_skip_space(&p);
    if (!strs_starts(p, match, ignorecase))
        return _false;
    p += strlen(match);
    if (strchr(follow, *p) && *p != 0) // make sure that it's the whole word..
        return _false;
    *s = p;
    return _true;
}

_bool strs_read_quoted(const char **s, char quote, char *buffer, _bool escape)
{
    const char *p = *s;
    strs_skip_space(&p);
    if (*p != quote)
        return _false;
    p++;
    char *pb = buffer;
    while (*p && *p != quote)
    {
        if (escape && *p == '\\') // escape..
            p++;
        *pb++ = *p++;
    }
    if (!*p)
    {
        printf("Attempting to parse an unterminated quoted string!");
        return _false;
    }
    p++;
    *pb = 0;
    *s = p;
    return _true;
}

_bool strs_read_int(const char **s, int *u)
{
    while (isspace(**s)) (*s)++;
    const char *ends = *s;
    *u = (int)strtol(*s, (char **)(&ends), 10); // this (char **) cast needs to be const_cast<char **> in C++
    if (ends && ends != *s)
    {
        *s = ends;
        return _true;
    }
    return _false;
}

_bool strs_read_uint(const char **s, unsigned *u)
{
    while (isspace(**s)) (*s)++;
    const char *ends = *s;
    *u = (int)strtoul(*s, (char **)(&ends), 10); // this (char **) cast needs to be const_cast<char **> in C++
    if (ends && ends != *s)
    {
        *s = ends;
        return _true;
    }
    return _false;
}

_bool strs_read_float(const char **s, float *u)
{
    while (isspace(**s)) (*s)++;
    const char *ends = *s;
    *u = (float)strtod(*s, (char **)(&ends)); // this (char **) cast needs to be const_cast<char **> in C++
    if (ends && ends != *s)
    {
        *s = ends;
        const char ind[] = "#IND";
        if (memicmp(*s, ind, sizeof(ind) - 1) == 0)
        {
            *u = 0.f; // QNaN, just set to zero for now.. $todo set properly to QNaN, please..
            *s += sizeof(ind) - 1;
            while (**s == '0') (*s)++;
        }
        return _true;
    }
    return _false;
}

_bool strs_read_double(const char **s, double *u)
{
    while (isspace(**s)) (*s)++;
    const char *ends = *s;
    *u = strtod(*s, (char **)(&ends)); // this (char **) cast needs to be const_cast<char **> in C++
    if (ends && ends != *s)
    {
        *s = ends;
        const char ind[] = "#IND";
        if (memicmp(*s, ind, sizeof(ind) - 1) == 0)
        {
            *u = 0.0; // QNaN, just set to zero for now.. $todo set properly to QNaN, please..
            *s += sizeof(ind) - 1;
            while (**s == '0') (*s)++;
        }
        return _true;
    }
    return _false;
}

void strs_replace_char(char *s, char f, char r)
{
    while (*s) { if (*s == f) *s = r; s++; }
}

void strs_replace_chars(char *s, const char *f, char r)
{
    while (*s) { if (strchr(f, *s)) *s = r; s++; }
}

_bool strs_isspace(const char *s)
{
    while (*s && isspace(*s)) s++;
    return !*s;
}

_bool strs_isdigit(const char *s)
{
    while (*s && isdigit(*s)) s++;
    return !*s;
}

_bool strs_isupper(const char *s)
{
    while (*s && isupper(*s)) s++;
    return !*s;
}

_bool strs_islower(const char *s)
{
    while (*s && isupper(*s)) s++;
    return !*s;
}

_bool strs_composed_of(const char *s, const char *of)
{
    while (*s && strchr(of, *s)) s++;
    return !*s;
}

static const char *strs_find_case(const char *s, const char *f)
{
    char c, sc;
    size_t len;

    if ((c = *f++) != 0)
    {
        len = strlen(f);
        do
        {
            do { if ((sc = *s++) == 0) return 0; }
            while (sc != c);
        }
        while (strncmp(s, f, len) != 0);
        s--;
    }
    return s;
}

static const char *strs_find_nocase(const char *s, const char *f)
{
    char c, sc;
    size_t len;

    if ((c = tolower(*f++)) != 0)
    {
        len = strlen(f);
        do
        {
            do { if ((sc = *s++) == 0) return 0; }
            while (tolower(sc) != c);
        }
        while (strnicmp(s, f, len) != 0);
        s--;
    }
    return s;
}

const char *strs_find(const char *s, const char *f, _bool ignorecase)
{
    return ignorecase ? strs_find_nocase(s, f) : strs_find_case(s, f);
}

char *strs_replace(const char *s, const char *f, const char *r, _bool ignorecase)
{
    unsigned count = strs_count(s, f, ignorecase);
    if (count == 0)
        return strs_strdup(s);
    size_t ssz = strlen(s), rsz = strlen(r), fsz = strlen(f);
    char *nstr = (char *)malloc((ssz + rsz * count + 4) * sizeof(char *));
    const char *sp    = s;
    char       *np    = nstr;
    const char *found = NULL;
    while (NULL != (found = strs_find(sp, f, ignorecase)))
    {
        size_t sz = found - sp;
        memcpy(np, sp, sz);
        memcpy(np + sz, r, rsz);
        np += sz + rsz;
        sp += sz + fsz;
        *np = '\0';
    }
    strcpy(np, sp);
    return nstr;
}

unsigned strs_count(const char *s, const char *f, _bool ignorecase)
{
    unsigned count = 0, fsz = (unsigned)strlen(f);
    while (*s && (s = strs_find(s, f, ignorecase))) { s += fsz; count++; }
    return count;
}

_bool strs_ends(const char *data, const char *substr, _bool ignorecase)
{
    if (!data || !substr) return _false;
    size_t dsz = strlen(data), ssz = strlen(substr), sz = _min_(dsz, ssz);
    if (sz < ssz) return _false;
    data += dsz - ssz;
    return (ignorecase ? memicmp(data, substr, sz) : memcmp(data, substr, sz)) == 0;
}

_bool strs_starts(const char *data, const char *substr, _bool ignorecase)
{
    if (!data || !substr) return _false;
    size_t dsz = strlen(data), ssz = strlen(substr), sz = _min_(dsz, ssz);
    if (sz < ssz) return _false;
    return (ignorecase ? memicmp(data, substr, sz) : memcmp(data, substr, sz)) == 0;
}

char **strs_split_char_of(const char *in, const char *of)
{
    char **splist = 0, *spstr = 0;
    int lin = (int)strlen(in), nspl = 0,
        j = 0, size = strs_count_char_of(in, of) + 2;
    _bool last = _false;

    spstr  = new_chars(lin + 1, _true);
    splist = new_char_ptrs(size, _true);

    for (int i = 0; i < lin; i++)
    {
        if (strchr(of, in[i]))
        {
            if (!last)
            {
                j = 0;
                if (strlen(spstr) != 0)
                {
                    splist[nspl] = new_chars(strlen(spstr) + 1, _true);
                    strcpy(splist[nspl], spstr);
                    nspl++;
                }
            }
            last = _true;
        }
        else
        {
            spstr[j++] = in[i];
            spstr[j] = 0;
            last = _false;
        }
    }
    if (j)
    {
        splist[nspl] = new_chars(strlen(spstr) + 1, _true);
        strcpy(splist[nspl], spstr);
        nspl++;
    }

    free(spstr);
    return splist;
}

char **strs_split_char_of_keep_empty(const char *in, const char *of)
{
    char **splist = 0, *spstr = 0;
    int lin = (int)strlen(in), nspl = 0,
        j = 0, size = strs_count_char_of(in, of) + 2;
    _bool last = _false;
    
    spstr  = new_chars(lin + 1, _true);
    splist = new_char_ptrs(size, _true);
    
    for (int i = 0; i < lin; i++)
    {
        if (strchr(of, in[i]))
        {
            j = 0;
            splist[nspl] = new_chars(strlen(spstr) + 1, _true);
            strcpy(splist[nspl], spstr);
            memset(spstr, 0, lin + 1);
            nspl++;
            last = _true;
        }
        else
        {
            spstr[j++] = in[i];
            spstr[j] = 0;
            last = _false;
        }
    }
    if (j)
    {
        splist[nspl] = new_chars(strlen(spstr) + 1, _true);
        strcpy(splist[nspl], spstr);
        nspl++;
    }
    
    free(spstr);
    return splist;
}

const char *strs_extension(const char *filename)
{
    if (!filename) return 0;
    const char *p = (const char *)strrchr(filename, '.');
    if (p) p++;
    return p;
}

char *strs_first_not_space(char *p)
{
    while (*p && isspace(*p)) p++;
    return *p ? p : 0;
}

char *strs_first_not_of(char *p, const char *of)
{
    while (*p && strchr(of, *p)) p++;
    return *p ? p : 0;
}

unsigned strs_count_char(const char *p, char c)
{
    unsigned count = 0;
    while (*p)
    {
        count += (c == *p) ? 1 : 0;
        p++;
    }
    return count;
}

unsigned strs_count_char_of(const char *p, const char *of)
{
    if (!*(of + 1))
        return strs_count_char(p, *of);
    unsigned count = 0;
    while (*p)
    {
        count += strchr(of, *p) != 0 ? 1 : 0;
        p++;
    }
    return count;
}

char *strs_first_of(char *p, const char *of)
{
    if (!*(of + 1)) return strchr(p, *of);
    while (*p && !strchr(of, *p)) p++;
    return *p ? p : 0;
}

char *strs_last_not_of(char *p, const char *of)
{
    while (*p && strchr(of, *p)) p++;
    return *p ? p : 0;
}

char *strs_last_of(char *p, const char *of)
{
    if (!*(of + 1)) return strrchr(p, *of);
    char *q = p + strlen(p) - 1;
    while (q > p && !strchr(of, *q)) q--;
    return *q ? q : 0;
}

void strs_destructive_rtrim(char *p)
{
    char *q = p + strlen(p) - 1;
    while (isspace(*q) && q > p) q--;
    *++q = 0;
}

void strs_destructive_ltrim(char *p)
{
    char *q = p;
    while (isspace(*q)) q++;
    if (q != p) strcpy(p, q);    
}

void strs_destructive_rtrim_of(char *p, const char *of)
{
    char *q = p + strlen(p) - 1;
    while (strchr(of, *q) && q > p) q--;
    *++q = 0;
}

void strs_destructive_ltrim_of(char *p, const char *of)
{
    char *q = p;
    while (strchr(of, *q)) q++;
    if (q != p) strcpy(p, q);
}

void strs_destructive_trim(char *p)
{
    strs_destructive_ltrim(p);
    strs_destructive_rtrim(p);
}

void strs_destructive_trim_of(char *p, const char *of)
{
    strs_destructive_ltrim_of(p, of);
    strs_destructive_rtrim_of(p, of);
}

void strs_destructive_trim_of_lr(char *p, const char *of_l, const char *of_r)
{
    if (of_l) strs_destructive_ltrim_of(p, of_l);
    if (of_r) strs_destructive_rtrim_of(p, of_r);
}

char *strs_trim(const char *p)
{
    if (!p) return 0;
    while (isspace(*p)) p++;
    size_t sz = strlen(p);
    char *q = new_chars(sz + 1, _false);
    strcpy(q, p);
    strs_destructive_rtrim(q);
    return q;
}

char *strs_trim_of(const char *p, const char *of)
{
    if (!p) return 0;
    while (strchr(of, *p)) p++;
    size_t sz = strlen(p);
    char *q = new_chars(sz + 1, _false);
    strcpy(q, p);
    strs_destructive_rtrim_of(q, of);
    return q;
}

void strs_destructive_transform(char *s, strs_char_transform_fn fn)
{
    while (*s) { *s = (char)fn(*s); s++; }
}

void strs_copy_transform(const char *s, char *t, strs_char_transform_fn fn)
{
    while (*s) { *t = (char)fn(*s); s++; t++; }
    *t = '\0';
}

void strs_destructive_toupper(char *s)         { return strs_destructive_transform(s, toupper); }
void strs_destructive_tolower(char *s)         { return strs_destructive_transform(s, tolower); }

void strs_copy_toupper(const char *s, char *t) { return strs_copy_transform(s, t, toupper); }
void strs_copy_tolower(const char *s, char *t) { return strs_copy_transform(s, t, tolower); }
