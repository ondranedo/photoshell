/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/29
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <core.h>

typedef enum {
    IMAGE_RGB,
    IMAGE_RGBA,
    IMAGE_BW
} ImageType;

typedef struct { u8 a; } PixelBW;
typedef struct { u8 r, g, b; } PixelRGB;
typedef struct { u8 r, g, b, a; } PixelRGBA;

typedef struct {
    u8* mem;
    u64 width, height;
    ImageType type;
    bool valid;
} Image;

typedef void(*ImageConvertFn)(u8* old, u128 old_size, u8* new, u128 new_size);

API Image image_construct(u128 width, u128 height, ImageType type);
API Image image_construct_cut(const Image* self, u128 ulx, u128 uly, u128 lrx, u128 lry);
API void  image_destruct(Image* self);

API void image_print(const Image* self);
API u128 image_type_size(const ImageType type);
API void image_convert(Image* self, ImageType type, ImageConvertFn convert_fn);
API void image_crop(Image* self, u128 ulx, u128 uly, u128 lrx, u128 lry);
API void image_place_over(Image* self, u128 x, u128 y, Image* other_img);
API void image_roll(Image* self, u128 left_to_right, u128 top_to_bottom);
API void image_resize(Image* self, u128 new_width, u128 new_height);


API u8*        image_get_pixel_raw(const Image* self, u64 x, u64 y);
API PixelBW*   image_get_pixel_bw(const Image* self, u64 x, u64 y);
API PixelRGB*  image_get_pixel_rgb(const Image* self, u64 x, u64 y);
API PixelRGBA* image_get_pixel_rgba(const Image* data, u64 x, u64 y);

API void image_set_pixel_raw(Image* self, u64 x, u64 y, u8* raw);

#endif //IMAGE_H
