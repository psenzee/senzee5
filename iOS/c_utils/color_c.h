//
//  color_c.h
//
//  Pure C code to parse and transform to/from HTML-style colors and floats and integers.

#import "types_c.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    double red, green, blue, alpha;
    
} real_color;
    
real_color color_make(double red, double green, double blue, double alpha);
    
_bool      color_parse_hex_reals(const char **s, double *red, double *green, double *blue, double *alpha);
_bool      color_parse_hex_struct(const char **s, real_color *color, _bool parse_alpha);
    
unsigned   color_rgba32_from_reals(double red, double green, double blue, double alpha);
unsigned   color_rgba32_from_struct(const real_color *color);

#ifdef __cplusplus
}
#endif