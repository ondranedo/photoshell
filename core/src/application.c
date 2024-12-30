/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#include <application.h>
#include <argument.h>
#include <image-tga.h>
#include <error.h>

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
        .command_handler = commandhandler_construct(),
        .running = false,
        .valid = true };

    argumentlist_validate(&app.argument_list);

    app.image = image_construct_tga(argumentlist_get(&app.argument_list, ARGUMENT_INPUT)->payload.string);

    image_convert(&app.image, IMAGE_BW, average);

    image_save_tga(&app.image, argumentlist_get(&app.argument_list, ARGUMENT_OUTPUT)->payload.string);

    errorhandler_handle();

    return app;
}

void application_destruct(Application* self) {
    argumentlist_destruct(&self->argument_list);
    commandhandler_destruct(&self->command_handler);
    image_destruct(&self->image);
}

void application_perform_command(Application* self, CommandType type, void* payload) {
    commandhandler_get_execute_fn(&self->command_handler, type)(self, payload);
}
