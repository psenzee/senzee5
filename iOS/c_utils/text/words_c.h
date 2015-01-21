// words_c - simple, fast word handling functions, Paul Senzee (c) 2014

#ifndef _WORDS_C_H
#define _WORDS_C_H

#include "pool_c.h"

#ifdef __cplusplus
extern "C" {
#endif
    
typedef int (*letter_transform_fn_t)(int ch);
typedef int (*word_callback_fn_t)(const char *word, void *context);
    
idx_pool_t *read_dictionary(const char *filename, int lowercase, int working_bytes);
idx_pool_t *read_dictionary_with_callback(const char *filename, int lowercase, int working_bytes, word_callback_fn_t callback);
int         read_dictionary_into_pool(const char *filename, idx_pool_t *pool, int lowercase);
int         write_index(idx_pool_t *pool, const char *outfile);
int         write_dictionary(const char *infile, const char *outfile, int lowercase, int working_bytes);
    

    
// transform is typically tolower, toupper, or NULL
int         read_file_words(const char *file,
                            word_callback_fn_t wordfn,
                            word_callback_fn_t punctuationfn,
                            letter_transform_fn_t transform, void *context);
typedef struct markov_t
{
    idx_pool_t  *sorted;
    void       **list;
    
} markov_t;

typedef int (*markov_iterate_callback_fn_t)(const char *a, const char *b, int count, void *context);
    
markov_t       *markov_create(idx_pool_t *sorted);
void            markov_free(markov_t *markov);
int             markov_add(markov_t *markov, const char *a, const char *b);
int             markov_iterate(markov_t *markov, markov_iterate_callback_fn_t iterator, void *context);

typedef enum MarkovOption_t
{
    MarkovOptionNone      = 0,
    MarkovOptionReverse   = 1,
    MarkovOptionUncommon  = 2
    
} MarkovOption;
    
typedef struct markov_state_t
{
    char     *previous;
    markov_t *markov;
    int       options;
        
} markov_state_t;
    
int             read_file_into_markov_state(const char *filename, markov_state_t *state, int lowercase);
int             write_markov_file(markov_t *markov, const char *filename);
    
markov_state_t *markov_state_create(markov_t *markov,  int options);
void            markov_state_free(markov_state_t *state);
void            markov_state_clear_previous_word(markov_state_t *state);
    
// sort of private
int             read_pool_word(const char *word, void *context);
int             read_markov_word(const char *word, void *context);
    
#ifdef __cplusplus
}
#endif

#endif // _WORDS_C_H
