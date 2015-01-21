#include "image_channel_c.h"

static inline int clamp_x(const image_channel_data *data, int x)
{
    int w = data->width - 1;
    return x < 0 ? 0 : ((x > w) ? w : x);
}

static inline int clamp_y(const image_channel_data *data, int y)
{
    int h = data->height - 1;
    return y < 0 ? 0 : ((y > h) ? h : y);
}

static inline int clamp_x_fns(const image_channel_mutator *data, int x)
{
    int w = data->width - 1;
    return x < 0 ? 0 : ((x > w) ? w : x);
}

static inline int clamp_y_fns(const image_channel_mutator *data, int y)
{
    int h = data->height - 1;
    return y < 0 ? 0 : ((y > h) ? h : y);
}

static inline int xy_inside(const image_channel_data *data, int x, int y)
{
    return x >= 0 && x < data->width && y >= 0 && y < data->height;
}

static inline img_real_t val(const image_channel_data *data, int x, int y)
{
    return (data->data)[y * data->width + x];
}

static inline void setval(image_channel_data *data, int x, int y, img_real_t value)
{
   (data->data)[y * data->width + x] = value;
}

img_real_t img_get_value(const image_channel_data *data, int x, int y)
{
    x = clamp_x(data, x); y = clamp_y(data, y);
    return val(data, x, y);
}

img_real_t img_get_value_fns(const image_channel_mutator *data, int x, int y)
{
    x = clamp_x_fns(data, x); y = clamp_y_fns(data, y);
    return (data->get)(data->context, x, y);
}

static inline double dval(const image_channel_data *data, int x, int y)
{
    return (double)img_get_value(data, x, y);
}

void img_set_value(image_channel_data *data, int x, int y, img_real_t value)
{
   if (xy_inside(data, x, y))
       setval(data, x, y, value);
}

// todo
//void       img_set_value_fns(image_channel_mutator *data, int x, int y, img_real_t value)
//{
//if (xy_inside(data, x, y))
//setval(data, x, y, value);
//}

img_real_t img_get_gaussian_blur3_value(const image_channel_data *data, int x, int y)
{
    double kernel[] = // gaussian
    {
        0.0751,    0.1238,    0.0751, 
        0.1238,    0.2042,    0.1238, 
        0.0751,    0.1238,    0.0751,
    };
    
    double value = 
    
      dval(data, x - 1, y - 1) * kernel[0] + dval(data, x, y - 1) * kernel[1] + dval(data, x + 1,  y - 1) * kernel[2] +
      dval(data, x - 1, y    ) * kernel[3] + dval(data, x, y    ) * kernel[4] + dval(data, x + 1,  y    ) * kernel[5] +
      dval(data, x - 1, y + 1) * kernel[6] + dval(data, x, y + 1) * kernel[7] + dval(data, x + 1,  y + 1) * kernel[8];
      
    return (img_real_t)value;
}

// todo
//img_real_t img_get_gaussian_blur3_value_fns(img_fn_get_value fn, const void *context, int x, int y)
//{
//}

void img_blur3(const image_channel_data *indata, image_channel_data *outdata)
{
    int w = indata->width  < outdata->width  ? indata->width  : outdata->width;
    int h = indata->height < outdata->height ? indata->height : outdata->height;
    for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++)
            img_set_value(outdata, i, j, img_get_gaussian_blur3_value(indata, i, j));
}

// todo
// void img_blur3_fns(image_channel_mutator *indata, image_channel_mutator *outdata)
//{
//}