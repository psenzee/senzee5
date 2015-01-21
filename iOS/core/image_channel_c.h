#ifndef _IMAGE_CHANNEL_C_H_
#define _IMAGE_CHANNEL_C_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef float img_real_t;

typedef img_real_t (*img_fn_get_value)(const void *context, int x, int y);
typedef void       (*img_fn_set_value)(void *context, int x, int y, img_real_t value);
    
typedef struct
{
    img_real_t *data;
    int         width;
    int         height;

} image_channel_data;
    
typedef struct
{
    img_fn_get_value  get;
    img_fn_set_value  set;
    int               width;
    int               height;
    void             *context;
    
} image_channel_mutator;

img_real_t img_get_value(const image_channel_data *data, int x, int y);
void       img_set_value(image_channel_data *data, int x, int y, img_real_t value);
    
img_real_t img_get_value_fns(const image_channel_mutator *data, int x, int y);
void       img_set_value_fns(image_channel_mutator *data, int x, int y, img_real_t value);
    
img_real_t img_get_gaussian_blur3_value(const image_channel_data *data, int x, int y);
img_real_t img_get_gaussian_blur3_value_fns(img_fn_get_value fn, const void *context, int x, int y);
    
void       img_blur3(const image_channel_data *indata, image_channel_data *outdata);
void       img_blur3_fns(image_channel_mutator *indata, image_channel_mutator *outdata);

#ifdef __cplusplus
}
#endif

#endif // _IMAGE_CHANNEL_C_H_
