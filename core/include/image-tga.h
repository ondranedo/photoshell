/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/29
 */

#ifndef IMAGE_TGA_H
#define IMAGE_TGA_H

#include <image.h>

typedef struct {
	i8 id_length;
	i8 color_map_type;
	i8 data_type_code;
	i16 color_map_origin;
	i16 color_map_length;
	i8 color_map_depth;
	i8 origin_x;
	i8 origin_y;
	i16 width;
	i16 height;
	i8 bits_per_pixel;
	i8 image_descriptor;
} TGAHeader;

API Image image_construct_tga(const char* path);
API void  image_save_tga(Image* image, const char* path);

#endif //IMAGE_TGA_H
