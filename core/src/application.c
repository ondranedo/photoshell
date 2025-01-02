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

void command_resize(Application* application, CommandResize* data) {
    image_resize(&application->image, data->w, data->h);
}

void command_crop(Application* application, CommandCrop* data) {
    image_crop(&application->image, data->x, data->y, data->x + data->w, data->y + data->h);
}

void command_copy(Application* application, CommandCopy* data) {
    Image image = image_construct_cut(&application->image, data->x1, data->y1, data->x1 + data->w, data->y1 + data->h);
    if(image.valid) {
        image_place_over(&application->image, data->y1, data->y2, &image);
        image_destruct(&image);
    }
}

void command_move(Application* application, CommandMove* data) {
    image_roll(&application->image,
        data->x < 0 ? -data->x : application->image.width - data->x,
        data->y < 0 ? -data->y : application->image.height - data->y
    );
}

void average(u8* old, u128 old_size, u8* new, u128 new_size) {
    u128 i = 0, sum = 0;
    if(old_size < new_size) return;
    for(; i < old_size; ++i) { sum+= old[i]; }
    for(i = 0; i < new_size; ++i) { new[i] = sum/old_size;}
}

void command_bw(Application* application, void*) {
    image_convert(&application->image, IMAGE_BW, average);
}

void command_info(Application* application, void*) {
    printf("W: %llu\tH: %llu\n", application->image.width, application->image.height);
}

void command_save(Application* application, void*) {
    image_save_tga(&application->image, argumentlist_get(&application->argument_list, ARGUMENT_OUTPUT)->payload.string);
}

void command_exit(Application* application, void* d) {
    command_save(application, d);
    application->running = false;
}

Application application_construct(int argc, char** argv) {
    Application app = {
        .argument_list = argumentlist_construct(argc, argv),
        .command_handler = commandhandler_construct(),
        .running = true, /* false in case of main loop */
        .valid = true };

    argumentlist_validate(&app.argument_list);
    errorhandler_handle();

    if(argumentlist_has(&app.argument_list, ARGUMENT_HELP)) {
        // TODO: Add Help
    }
    else if(argumentlist_has(&app.argument_list, ARGUMENT_VERSION)) {
        printf("Version 0.1\n");
    } else if(argumentlist_has(&app.argument_list, ARGUMENT_INPUT)) {
        commandhandler_set_execute_fn(&app.command_handler, COMMAND_RESIZE, (CommandFn)command_resize);
        commandhandler_set_execute_fn(&app.command_handler, COMMAND_CROP, (CommandFn)command_crop);
        commandhandler_set_execute_fn(&app.command_handler, COMMAND_COPY, (CommandFn)command_copy);
        commandhandler_set_execute_fn(&app.command_handler, COMMAND_MOVE, (CommandFn)command_move);
        commandhandler_set_execute_fn(&app.command_handler, COMMAND_BW, (CommandFn)command_bw);
        commandhandler_set_execute_fn(&app.command_handler, COMMAND_INFO, (CommandFn)command_info);
        commandhandler_set_execute_fn(&app.command_handler, COMMAND_SAVE, (CommandFn)command_save);
        commandhandler_set_execute_fn(&app.command_handler, COMMAND_EXIT, (CommandFn)command_exit);
        app.image = image_construct_tga(argumentlist_get(&app.argument_list, ARGUMENT_INPUT)->payload.string);
    }

    errorhandler_handle();

    return app;
}

void application_destruct(Application* self) {
    argumentlist_destruct(&self->argument_list);
    commandhandler_destruct(&self->command_handler);
    image_destruct(&self->image);
}

void application_perform_command(Application* self, CommandType type, void* payload) {
    CommandFn fn = commandhandler_get_execute_fn(&self->command_handler, type);
    if(fn)
        fn(self, payload);
}
