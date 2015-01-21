// Pure C String and Parsing Utilities, Paul D. Senzee (c) 2000-2011

#ifndef _STRS_C_H
#define _STRS_C_H

#include <string.h>

#define _bool  int
#define _true  1
#define _false 0

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIN32
#define stricmp  strcasecmp
#define strnicmp strncasecmp
int memicmp(const void *s1, const void *s2, size_t n);
#endif

// strs_tmp_buffer/strs_free_tmp_buffer are fast, preferably using stack storage if small
// but they only allow one buffer allocated at a time
char         *strs_tmp_buffer(size_t size);
void          strs_free_tmp_buffer(const char *buffer);
    
char         *strs_strdup(const char *s);
int           strs_strlistcount(const char **p);
char        **strs_strlistdup(const char **p);
void          strs_destroy_strlist(char **p);

void          strs_skip_space(const char **s);
void          strs_skip_space_not_newline(const char **s);

// the strs_is*() and strs_read*() functions below skip preceeding whitespace

// strs_is*() - is it a string followed by '\0' or:
//  a space (is),
//  one of the characters specified (is_and_followed_by_any),
//  or not one of the characters specified (is_and_not_followed_by_any)?

_bool         strs_is(const char **s, const char *match, _bool ignorecase);
_bool         strs_is_and_followed_by_space(const char **s, const char *match, _bool ignorecase);
_bool         strs_is_and_followed_by_any_of(const char **s, const char *match, const char *after, _bool ignorecase);
_bool         strs_is_and_not_followed_by_any_of(const char **s, const char *match, const char *after, _bool ignorecase);

_bool         strs_read_quoted(const char **s, char quote, char *buffer, _bool escape);
_bool         strs_read_identifier_of(const char **s, char *buffer, const char *of);
_bool         strs_read_float(const char **s, float *u);
_bool         strs_read_double(const char **s, double *u);
_bool         strs_read_int(const char **s, int *u);
_bool         strs_read_uint(const char **s, unsigned *u);

const char   *strs_find(const char *s, const char *f, _bool ignorecase);
unsigned      strs_count(const char *s, const char *f, _bool ignorecase);
_bool         strs_ends(const char *data, const char *substr, _bool ignorecase);
_bool         strs_starts(const char *data, const char *substr, _bool ignorecase);

char        **strs_split_char_of(const char *in, const char *of);
char        **strs_split_char_of_keep_empty(const char *in, const char *of);

const char   *strs_extension(const char *filename);

void          strs_replace_char(char *s, char f, char r);
void          strs_replace_chars(char *s, const char *f, char r);
char         *strs_replace(const char *s, const char *f, const char *r, _bool ignorecase);

_bool         strs_isspace(const char *s);
_bool         strs_isdigit(const char *s);
_bool         strs_isupper(const char *s);
_bool         strs_islower(const char *s);

unsigned      strs_count_char(const char *p, char c);
unsigned      strs_count_char_of(const char *s, const char *of);

_bool         strs_composed_of(const char *s, const char *of);
char         *strs_first_not_of(char *p, const char *of);
char         *strs_first_of(char *p, const char *of);
char         *strs_last_not_of(char *p, const char *of);
char         *strs_last_of(char *p, const char *of);
char         *strs_first_not_space(char *p);

void          strs_destructive_rtrim(char *p);
void          strs_destructive_ltrim(char *p);
void          strs_destructive_rtrim_of(char *p, const char *of);
void          strs_destructive_ltrim_of(char *p, const char *of);

void          strs_destructive_trim(char *p);
void          strs_destructive_trim_of(char *p, const char *of);
void          strs_destructive_trim_of_lr(char *p, const char *of_l, const char *of_r);

char         *strs_trim(const char *p);
char         *strs_trim_of(const char *p, const char *of);

typedef int (*strs_char_transform_fn)(int ch);
    
void          strs_destructive_transform(char *s, strs_char_transform_fn fn);

void          strs_destructive_toupper(char *s);
void          strs_destructive_tolower(char *s);
    
void          strs_copy_transform(const char *s, char *t, strs_char_transform_fn fn);
    
void          strs_copy_toupper(const char *s, char *t);
void          strs_copy_tolower(const char *s, char *t);
    
#ifdef __cplusplus
}
#endif

#endif // _STRS_H