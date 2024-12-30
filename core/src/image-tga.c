/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/29
 */

#include <image-tga.h>
#include <error.h>

// TODO: Remove
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Image _construct_tga(TGAHeader* header) {
    ImageType type;
    Image image;
    switch(header->bits_per_pixel/8) {
        case 1: type = IMAGE_BW; break;
        case 3: type = IMAGE_RGB; break;
        case 4: type = IMAGE_RGBA; break;
        default:
            error_throw(ERROR_RUNT, "Unknown TGA bits_per_pixel", true);
        return image_construct(1, 1, IMAGE_BW);
    }
    image = image_construct(header->width, header->height, type);
    return image;
}

TGAHeader _construct_header(Image* image) {
    TGAHeader header = { 0 };
    header.bits_per_pixel = 8*image_type_size(image->type);
    header.width = image->width;
    header.height = image->height;
    return header;
}

Image image_construct_tga(const char* path) {
    Image image = { 0 };
    TGAHeader header;

    FILE* file = fopen(path, "rb");
    if(!file) {
        error_throw(ERROR_RUNT, "Failed to open input file.", true);
        return image_construct(1, 1, IMAGE_RGB);
    }
    fread(&header, sizeof(TGAHeader), 1, file);
    image = _construct_tga(&header);
    fread(image.mem, image.width*image.height*image_type_size(image.type), 1, file);
    fclose(file);

    return image;
}

API void  image_save_tga(Image* image, const char* path) {
    FILE* file = fopen(path, "wb");
    TGAHeader header;
    if(!file) {
        error_throw(ERROR_RUNT, "Failed to open output file.", true);
        return;
    }
    header = _construct_header(image);
    header.data_type_code = 2;
    fwrite(&header, sizeof(TGAHeader), 1, file);
    fwrite(image->mem, header.width*header.height*header.bits_per_pixel/8, 1, file);
    fclose(file);
}