// words_cpp - simple, fast word handling functions, Paul Senzee (c) 2014

#include "words_c.h"
#include "pool_c.h"
#include "strs_c.h"
#include "mapint_c.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int null_transform(int ch) { return ch; }

int markov_item_iterate(markov_t *m, int index, markov_iterate_callback_fn_t iterator, void *context);

int read_file_words(const char *filename,
                    word_callback_fn_t wordfn,
                    word_callback_fn_t puntuactionfn,
                    letter_transform_fn_t transform, void *context)
{
    const int MB = 1024 * 1024;
    const int MAX_WORD_SIZE = 1 * MB;
    
    if (!transform) transform = null_transform;
    
    FILE *file = fopen(filename, "r");
    if (!file)
        return 0;
    char punctuation[2] = "\0\0";
    char *word = (char *)malloc(MAX_WORD_SIZE);
    char *pointer = word;
    int ch = 0;
    while ((ch = fgetc(file)) != EOF)
    {
        if (isalpha(ch) ||
            (pointer != word && ch == '\'')) // allow apostrophes, but not words starting with them
            *pointer++ = (char)transform(ch);
        else
        {
            if (ispunct(ch) && puntuactionfn)
            {
                punctuation[0] = (char)ch;
                puntuactionfn(punctuation, context);
                punctuation[0] = '\0';
            }
            if (pointer != word) // we did read alphas
            {
                *pointer = '\0';
                pointer = word;
                if (!wordfn(pointer, context))
                {
                    free(word);
                    fclose(file);
                    return 0;
                }
                *pointer = '\0';
            }
        }
    }
    free(word);
    fclose(file);
    
    return 1;
}

int read_pool_word(const char *word, void *context)
{
    idx_pool_t *pool = (idx_pool_t *)context;
    if (!idx_pool_copy_str(pool, word))
    {
        fprintf(stderr, "\n\nread_pool_word: pool size (%lu) too small; can't allocate.\n\n",
                pool->size + pool->index_size);
        return 0;
    }
    return 1;
}

int read_markov_word(const char *word, void *context)
{
    markov_state_t *state = (markov_state_t *)context;
    markov_state_add_word(state, word); // we ignore the return value
    return 1;
}

int read_markov_punctuation(const char *word, void *context)
{
    markov_state_t *state = (markov_state_t *)context;
    int ch = *word;
    if (ch == '.' || ch == ':' || ch == ';') // restart chain on hard punctuation
        ;//       markov_state_clear_previous_word(state);
    return 1;
}

int read_file_into_markov_state(const char *filename, markov_state_t *state, int lowercase)
{
    int result = read_file_words(filename, read_markov_word, read_markov_punctuation, lowercase ? tolower : NULL, state);
    markov_state_clear_previous_word(state);
    return result;
}

int read_dictionary_into_pool(const char *filename, idx_pool_t *pool, int lowercase)
{
    return read_file_words(filename, read_pool_word, NULL, lowercase ? tolower : NULL, pool);
}

idx_pool_t *read_dictionary(const char *infile, int lowercase, int working_bytes)
{
    idx_pool_t *pool = idx_pool_create(working_bytes / 2, working_bytes / 2);
    if (!read_dictionary_into_pool(infile, pool, lowercase))
    {
        idx_pool_free(pool);
        return NULL;
    }
    idx_pool_t *cs = idx_pool_consolidate_strings(pool);
    idx_pool_free(pool);
    return cs;
}

idx_pool_t *read_dictionary_with_callback(const char *infile, int lowercase, int working_bytes, word_callback_fn_t wordfn)
{
    idx_pool_t *pool = idx_pool_create(working_bytes / 2, working_bytes / 2);
    if (!read_file_words(infile, wordfn, NULL, lowercase ? tolower : NULL, pool))
    {
        idx_pool_free(pool);
        return NULL;
    }
    idx_pool_t *cs = idx_pool_consolidate_strings(pool);
    idx_pool_free(pool);
    return cs;
}

int write_index(idx_pool_t *pool, const char *outfile)
{
    FILE *file = fopen(outfile, "w");
    if (!file)
        return 0;
    size_t count = idx_pool_index_count(pool);
    for (size_t i = 0; i < count; i++)
    {
        const char *item = (char *)idx_pool_item_at_index(pool, i);
        size_t size = strlen(item);
        fwrite(item, 1, size, file);
        fwrite("\n", 1, 1, file);
    }
    fclose(file);
    return 1;
}

int write_dictionary(const char *infile, const char *outfile, int lowercase, int working_bytes)
{
    idx_pool_t *pool = read_dictionary(infile, lowercase, working_bytes);
    if (!pool)
        return 0;
    int result = write_index(pool, outfile);
    idx_pool_free(pool);
    return result;
}

extern void markov_item_free(void *map);
extern int  markov_item_increment(markov_t *markov, int indexa, int indexb);

markov_t *markov_create(idx_pool_t *sorted)
{
    markov_t *m = (markov_t *)malloc(sizeof(markov_t));
    memset(m, 0, sizeof(markov_t));
    m->sorted = sorted;
    size_t count = idx_pool_index_count(sorted), sz = count * sizeof(void *);
    m->list = (void **)malloc(sz);
    memset(m->list, 0, sz);
    return m;
}

int markov_add(markov_t *m, const char *a, const char *b)
{
    if (!m || !a || !*a || !b || !*b)
        return 0;
    int indexa = idx_pool_bsearch_find(m->sorted, a);
    if (indexa < 0) return 0;
    int indexb = idx_pool_bsearch_find(m->sorted, b);
    if (indexb < 0) return 0;
    return markov_item_increment(m, indexa, indexb);
}

void markov_free(markov_t *m)
{
    if (m)
    {
        size_t count = idx_pool_index_count(m->sorted);
        for (int i = 0; i < count; i++)
            markov_item_free(m->list[i]);
        free(m->list);
        memset(m, 0, sizeof(markov_t));
        free(m);
    }
}

void markov_item_free(void *map)
{
    mapint_free(map);
}

int markov_item_increment(markov_t *m, int indexa, int indexb)
{
    void *map = m->list[indexa];
    if (!map)
        m->list[indexa] = map = mapint_create();
    return mapint_increment(map, indexb);
}

int markov_iterate(markov_t *markov, markov_iterate_callback_fn_t iterator, void *context)
{
    size_t count = idx_pool_index_count(markov->sorted);
    printf("markov_iterate found %ul items.\n", count);
    for (size_t i = 0; i < count; i++)
        if (!markov_item_iterate(markov, i, iterator, context))
            return 0;
    printf("markov_iterate iterated %ul items.\n", count);

    return 1;
}

markov_state_t *markov_state_create(markov_t *markov, int options)
{
    const int WORD_SIZE = 1 * 1024 * 1024;
    markov_state_t *state = (markov_state_t *)malloc(sizeof(markov_state_t));
    memset(state, 0, sizeof(markov_state_t));
    state->markov = markov;
    state->options = options;
    state->previous = (char *)malloc(WORD_SIZE * sizeof(char));
    memset(state->previous, 0, WORD_SIZE * sizeof(char));
    return state;
}

void markov_state_free(markov_state_t *state)
{
    if (state)
    {
        free(state->previous);
        memset(state, 0, sizeof(markov_state_t));
        free(state);
    }
}

static int qsort_string_compare_ascend(const void *a, const void *b)
{
    const char *p = *(const char **)a, *q = *(const char **)b;
    return stricmp(p, q);
}

int is_common_word(const char *word)
{
    static int    common_count = 0;
    static size_t length = 0;
    if (!word || !*word)
        return 0;
    static const char *common[] =
    {
        "a", "about", "after", "all", "also", "am", "an", "and", "any", "are", "as", "at", "away",
        "back", "be", "because", "beside", "besides", "but", "by", "came", "can", "come", "comes", "coming", "could",
        "did", "do", "does", "doesn't", "doing", "don't", "even", "first", "for", "from", "gave",
        "get", "gets", "give", "given", "gives", "giving", "go", "goes", "going", "good", "got",
        "gotten", "had", "has", "have", "having", "he", "her", "him", "his", "how", "i", "if",
        "in", "into", "is", "it", "it's", "its", "just", "knew", "know", "knowing", "knows",
        "like", "liked", "likes", "liking", "look", "looked", "looking", "looks", "made", "make",
        "makes", "making", "me", "most", "my", "new", "no", "not", "now", "of", "on", "one", "only",
        "or", "other", "our", "out", "over", "people", "said", "saw", "say", "saying", "says", "see",
        "seeing", "sees", "she", "so", "some", "still", "take", "takes", "taking", "than", "that", "the", "thee",
        "their", "them", "then", "there", "these", "they", "thing", "things", "think", "thinking", "thinks", "this",
        "thou", "thought", "thy", "time", "to", "together", "too", "took", "toward", "two", "unto",
        "up", "upon", "us", "use", "used", "uses",
        "using", "want", "wanted", "wanting", "wants", "was", "way", "we", "well", "went", "what", "when",
        "which", "while", "whiles", "whilst", "who", "why", "will", "wilt", "with", "within", "won't", "work",
        "worked", "working", "works", "would", "wouldn't", "ye", "yea", "yeah", "year", "you", "your", NULL
    };
    if (!common_count)
    {
        const char **p = (const char **)common;
        while (*p)
        {
            size_t len = strlen(*p);
            if (len > length)
                length = len;
            common_count++;
            p++;
        }
    }
    if (strlen(word) > length)
        return 0;
    const char **find = (const char **)&word;
    char **found = (char **)bsearch(find, common, common_count, sizeof(const char *), qsort_string_compare_ascend);
    return found && *found;
}

int markov_state_add_word(markov_state_t *state, const char *word)
{
    int result = 0;
    if (!state || !word || !*word || ((state->options & MarkovOptionUncommon) && is_common_word(word)))
        return 0;
    if (strlen(state->previous) && strlen(word))
    {
        if (state->options & MarkovOptionReverse)
            result = markov_add(state->markov, word, state->previous);
        else
            result = markov_add(state->markov, state->previous, word);
    }
    strcpy(state->previous, word);
    return result;
}

void markov_state_clear_previous_word(markov_state_t *state)
{
 //   if (state)
 //       *(state->previous) = '\0';
}

int markov_write_file_iterator(const char *a, const char *b, int count, void *context)
{
    FILE *file = (FILE *)context;
    if (count > 3)
        fprintf(file, "%05d:%s:%s\n", count, a, b);
    return 1;
}

int write_markov_file(markov_t *markov, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
        return 0;
    markov_iterate(markov, markov_write_file_iterator, file);
    fclose(file);
    return 1;
}

typedef struct markov_item_iterate_context_t
{
    idx_pool_t                   *sorted;
    const char                   *first;
    markov_iterate_callback_fn_t  iterator;
    void                         *iterator_context;
    
} markov_item_iterate_context_t;

int markov_item_iterate_iterator(int key, int value, void *map, void *context)
{
    markov_item_iterate_context_t *item_context = (markov_item_iterate_context_t *)context;
    const char *second = (const char *)idx_pool_item_at_index(item_context->sorted, key);
    return (item_context->iterator)(item_context->first, second, value, item_context->iterator_context);
}

int markov_item_iterate(markov_t *m, int index, markov_iterate_callback_fn_t iterator, void *context)
{
    void *map = m->list[index];
    if (map)
    {
        markov_item_iterate_context_t item_context;
        item_context.sorted = m->sorted;
        item_context.first  = (const char *)idx_pool_item_at_index(m->sorted, index);
        item_context.iterator = iterator;
        item_context.iterator_context = context;
        return mapint_iterate(map, markov_item_iterate_iterator, &item_context);
    }
    return 1;
}


