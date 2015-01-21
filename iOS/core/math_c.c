#include "math_c.h"
#include <math.h>
#include <strings.h>

const double MATHC_PI = 3.141592653589793;

double clamp(double value, double lo, double hi)
{
    return value > hi ? hi : (value < lo ? lo : value);
}

double scale_to_01(double value, double lo, double hi)
{
    return (value - lo) / (hi - lo);
}

double scale_from_01(double value, double lo, double hi)
{
    return ((hi - lo) * value) + lo;
}

double interpolate(double value, double lo, double hi)
{
    return scale_from_01(value, lo, hi);
}

double rescale(double value, double from_lo, double from_hi, double to_lo, double to_hi)
{
    return scale_to_01(
                       scale_from_01(value, from_lo, from_hi),
                       to_lo, to_hi);
}

double sinc_unnorm(double x)
{
    return sin(x) / x;
}

double sinc_norm(double x)
{
    double pi_x = MATHC_PI * x;
    return sin(pi_x) / pi_x;
}

double ease_linear(double t, const ease_context_t *context)
{
    return t;
}

double ease_bounce(double t, const ease_context_t *context)
{
    return pow(1.0 - sinc_norm(t * context->domain), context->power);
}

double ease_reverse_bounce(double t, const ease_context_t *context)
{
    return 1.0 - pow(1.0 - sinc_norm((1.0 - t) * context->domain), context->power);
}

double ease_power(double t, const ease_context_t *context)
{
    return pow(t, context->power);
}
