#ifndef _MATH_C_H
#define _MATH_C_H

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct
{
    double domain;
    double power;
    
} ease_context_t;
    
typedef double (*ease_callback_fn_t)(double, const ease_context_t *);

double interpolate(double value, double lo, double hi);
    
double ease_linear(double t, const ease_context_t *context);
double ease_bounce(double t, const ease_context_t *context);
double ease_reverse_bounce(double t, const ease_context_t *context);
double ease_power (double t, const ease_context_t *context);

#ifdef __cplusplus
}
#endif

#endif // _MATH_C_H
