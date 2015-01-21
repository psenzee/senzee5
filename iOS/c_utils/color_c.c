//  color_c.c

#include "color_c.h"
#include <ctype.h>

real_color color_make(double red, double green, double blue, double alpha)
{
    real_color rc = { red, green, blue, alpha };
    return rc;
}

static int hex_digit(char c)
{
    c = toupper(c);
    if ((c < '0' || c > '9') && (c < 'A' || c > 'F'))
        return -1;
    return (int)(c <= '9' ? (c - '0') : ((c - 'A') + 10));
}

static int hex(const char **s, int sz)
{
    if (!s) return -1;
    const char *p = *s;
    int v = 0, u = 0;
    char c = 0;
    for (int i = 0; i < sz; i++, p++)
    {
        if (!(c = *p) || (u = hex_digit(c)) == -1)
            return -1;
        v *= 16;
        v += u;
    }
    *s = p;
    return v;
}

_bool color_parse_hex_reals(const char **s, double *red, double *green, double *blue, double *alpha)
{
    const char *p = *s;
    _bool hasalpha = _false;
    char c = *p++;
    // only allow alpha extension ('%' instead of '#') if caller passed in a place to put the alpha
    if ((c != '#' && c != '%') || (c == '%' && !alpha))
        return _false;
    
    hasalpha = (c == '%'); // extension that allows us to specify an alpha channel
    if (alpha)
        *alpha = hasalpha ? 0.0 : 1.0;   // if no alpha, set it to full
    
    int u = 0;
    
    if ((u = hex(&p, 2)) == -1) return _false;
    *red   = u / 255.0;
    
    if ((u = hex(&p, 2)) == -1) return _false;
    *green = u / 255.0;
    
    if ((u = hex(&p, 2)) == -1) return _false;
    *blue  = u / 255.0;
    
    if (hasalpha)
    {
        if ((u = hex(&p, 2)) == -1) return _false;
        *alpha = u / 255.0;
    }
    *s = p;
    return _true;
}

_bool color_parse_hex_struct(const char **s, real_color *color, _bool parse_alpha)
{
    if (!parse_alpha)
    color->alpha = 1.0;
    return color_parse_hex_reals(s, &(color->red), &(color->green), &(color->blue), parse_alpha ? &(color->alpha) : NULL);
}

static inline unsigned color_channel(double value, int shift)
{
    return ((unsigned)(value * 255.0) & 0x0ff) << shift;
}

unsigned color_rgba32_from_reals(double red, double green, double blue, double alpha)
{
    return color_channel(red, 24) | color_channel(green, 16) |
           color_channel(blue, 8) | color_channel(alpha,  0);
}

unsigned color_rgba32_from_struct(const real_color *color)
{
    return color_rgba32_from_reals(color->red, color->green, color->blue, color->alpha);
}
