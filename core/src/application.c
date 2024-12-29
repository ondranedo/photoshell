/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#include <application.h>
#include <argument.h>
#include <image.h>

// TODO: Remove
#include <stdio.h>

void average(u8* old, u128 old_size, u8* new, u128 new_size) {
    u128 i = 0, sum = 0;
    if(old_size < new_size) return;
    for(; i < old_size; ++i) { sum+= old[i]; }
    for(i = 0; i < new_size; ++i) { new[i] = sum/old_size;}
}

Application application_construct(int argc, char** argv) {
    Application app = {
        .argument_list = argumentlist_construct(argc, argv),
        .running = false,
        .valid = true };

    argumentlist_validate(&app.argument_list);

    Image img = image_construct();
    image_convert(&img, IMAGE_BW, average);

    image_resize(&img, img.width*2, img.height);

    image_print(&img);

    image_destruct(&img);

    errorhandler_handle();

    return app;
}

void application_destruct(Application* self) {
    argumentlist_destruct(&self->argument_list);
}