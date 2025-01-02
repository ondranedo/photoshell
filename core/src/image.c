/*
 *  Ondrej Nedojedly
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/29
 */

#include <image.h>

// TODO: Remove
#include <stdlib.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

Image image_construct(u128 width, u128 height, ImageType type) {
    Image image = {
        .mem = malloc(width*height*image_type_size(type)*sizeof(u8)),
        .type = type,
        .height = height,
        .width = width,
        .valid = true
    };

    return image;
}

Image image_construct_cut(const Image* self, u128 ulx, u128 uly, u128 lrx, u128 lry) {
    Image image = { 0 };
    u128 j, i;
    //lrx+=1;
    //lry+=1;
    if(ulx >= lrx || uly >= lry) {
        error_throw(ERROR_RUNT, "Upper left corner invalid position relative to lower right corner.", false);
        image.valid = false;
        return image;
    }

    if(lrx > self->width || lry > self->height) {
        error_throw(ERROR_RUNT, "Lower right corner outside image.", false);
        image.valid = false;
        return image;
    }
    image = *self;
    image.width = lrx - ulx;
    image.height = lry - uly;
    image.mem = malloc(image_type_size(image.type)*image.width*image.height*sizeof(u8));

    for(i = uly; i < self->height && i < lry; ++i) {
        for(j = ulx; j < self->width && j < lrx; ++j) {
            image_set_pixel_raw(&image, j - ulx, i - uly, image_get_pixel_raw(self, j, i));
        }
    }

    return image;
}

void image_destruct(Image* self) {
    free (self->mem);
}

void image_print(const Image* self) {
    u128 n, j, i = 0;
    for(; i < self->height; ++i) {
        for(j = 0; j < self->width; ++j) {
            printf("[");
            for(n = 0; n < image_type_size(self->type); ++n) {
                printf("%02X", image_get_pixel_raw(self, j, i)[n]);
            }
            printf("]");
        }
        printf("\n");
    }
}

u128 image_type_size(ImageType type) {
    switch (type) {
        case IMAGE_BW: return sizeof(PixelBW)/sizeof(u8);
        case IMAGE_RGB: return sizeof(PixelRGB)/sizeof(u8);
        case IMAGE_RGBA: return sizeof(PixelRGBA)/sizeof(u8);
    }

    error_throw(ERROR_RUNT, "Unknown ImageType", false);
    return 0;
}

void image_convert(Image* self, ImageType new_type, ImageConvertFn convert_fn) {
    u128 n, j, i = 0;
    Image tmp = *self;
    u8* tmp_raw = 0, *old_raw;

    if(image_type_size(new_type) - image_type_size(self->type) == 0) { self->type = new_type; return; }
    tmp.mem = malloc(image_type_size(new_type)*sizeof(u8)*self->width*self->height);
    tmp.type = new_type;
    tmp_raw = malloc(image_type_size(new_type)*sizeof(u8));
    n = image_type_size(new_type) < image_type_size(self->type)?
        image_type_size(new_type):
        image_type_size(self->type);

    for(; i < self->height; ++i) {
        for(j = 0; j < self->width; ++j) {

            memset(tmp_raw, 0x00, image_type_size(new_type));
            old_raw = image_get_pixel_raw(self, j, i);
            memcpy(tmp_raw, old_raw, n);
            image_set_pixel_raw(&tmp, j, i, tmp_raw);
            if(convert_fn)
                convert_fn(
                    image_get_pixel_raw(self, j, i),image_type_size(self->type),
                    image_get_pixel_raw(&tmp, j, i),image_type_size(tmp.type));
        }
    }

    free(tmp_raw);
    image_destruct(self);
    *self = tmp;
}

void image_crop(Image* self, u128 ulx, u128 uly, u128 lrx, u128 lry) {
    Image image = image_construct_cut(self, ulx, uly, lrx, lry);
    if(image.valid) {
        image_destruct(self);
        *self = image;
    } else
        image_destruct(&image);
}

void image_place_over(Image* self, u128 x, u128 y, Image* other_img) {
    u128 j, i = y;

    for(; i < self->height; ++i) {
        for(j = x; j < self->width; ++j) {
            if((i - y) < other_img->height && (j - x) < other_img->width) {
                u8* raw = image_get_pixel_raw(other_img, j - x,i - y);
                image_set_pixel_raw(self, j, i, raw);
            }
        }
    }
}

void image_roll_left(Image* self, u128 count) {
    image_roll(self, count, 0);
}

void image_roll_up(Image* self, u128 count) {
    image_roll(self, 0, count);
}

void image_roll(Image* self, u128 left_to_right, u128 top_to_bottom) {
    Image copy = image_construct_cut(self, 0, 0, self->width, self->height);
    u128 j, i = 0;

    for(; i < self->height; ++i) {
        for(j = 0; j < self->width; ++j) {
            u8* raw = image_get_pixel_raw(self,
                (j + left_to_right)%self->width,
                (i + top_to_bottom)%self->height);
            image_set_pixel_raw(&copy, j, i, raw);
        }
    }

    image_destruct(self);
    *self = copy;
}

void image_resize(Image* self, u128 new_width, u128 new_height) {
    Image image = *self;
    u128 j, i = 0, jj, ii;
    f64 width_scale = (f64)self->width/(f64)new_width,
        height_scale = (f64)self->height/(f64)new_height;

    image.width = new_width;
    image.height = new_height;
    image.mem = malloc(image_type_size(image.type)*image.width*image.height*sizeof(u8));

    for(; i < image.height; i++) {
        for(j = 0; j < image.width; ++j) {
            jj = floor(((f64)(j))*width_scale);
            ii = floor(((f64)(i))*height_scale);
            u8* raw = image_get_pixel_raw(self, jj, ii);
            image_set_pixel_raw(&image, j, i, raw);
        }
    }

    image_destruct(self);
    *self = image;
}

u8* image_get_pixel_raw(const Image* self, u64 x, u64 y) {
    if(x > self->width || y > self->height) return self->mem;
    return self->mem + (self->width*image_type_size(self->type)*y + image_type_size(self->type)*x);
}

PixelBW* image_get_pixel_bw(const Image* data, u64 x, u64 y) {
    return (PixelBW*)image_get_pixel_raw(data, x, y);
}

PixelRGB* image_get_pixel_rgb(const Image* data, u64 x, u64 y) {
    return (PixelRGB*)image_get_pixel_raw(data, x, y);
}

PixelRGBA* image_get_pixel_rgba(const Image* data, u64 x, u64 y) {
    return (PixelRGBA*)image_get_pixel_raw(data, x, y);
}

void image_set_pixel_raw(Image* self, u64 x, u64 y, u8* raw) {
    u128 n = 0;
    for(; n < image_type_size(self->type); ++n)
        memset(image_get_pixel_raw(self, x, y) + n, raw[n], 1);
}