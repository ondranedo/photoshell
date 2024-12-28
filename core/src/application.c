/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#include <application.h>
#include <arguments.h>

// TODO: Remove
#include <stdio.h>

Application application_construct(int argc, char** argv) {
    Application app = {
        .argument_list = argumentlist_construct(argc, argv),
        .running = false,
        .valid = true };

    argumentlist_validate(&app.argument_list);

    errorhandler_handle();

    return app;
}

void application_run(Application* self) {
    argumentlist_print(&self->argument_list);
}

void application_destruct(Application* self) {
    argumentlist_destruct(&self->argument_list);
}