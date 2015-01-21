/* random_c.c */

#include "random_c.h"

static int     RAND_MAX_INT    = 0x007fffffff;
static int64_t RAND_MAX_INT64  = (int64_t)0x007fffffff;
static double  RAND_MAX_DOUBLE = (double)0x007fffffff;

random_t random_create(int64_t seed)
{
    random_t r;
    r.state = seed;
    return r;
}

int64_t random_max_int64()
{
    return RAND_MAX_INT64;
}

int random_max_int()
{
    return RAND_MAX_INT;
}

int random_integer(random_t *state)
{
//  return (int)(((state = state * 214013ull + 2531011ull) >> 16) & 0x007fffffffull);
    return (int)(((state->state = state->state * 22801764703ULL + 2038075387ULL) >> 16) & 0x007fffffffULL);
}

double random_real(random_t *state)
{
    return random_integer(state) / RAND_MAX_DOUBLE;
}