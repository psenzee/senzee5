#ifndef _RANDOM_C_H_
#define _RANDOM_C_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef long long int64_t;

typedef struct random_t
{
    int64_t state;
    
} random_t;

random_t random_create(int64_t seed);
int64_t  random_max_int64();
int      random_max_int();

int      random_integer(random_t *state);
double   random_real(random_t *state);

#ifdef __cplusplus
}
#endif

#endif /* _RANDOM_C_H_ */
