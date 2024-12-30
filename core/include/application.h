/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */



#ifndef APPLICATION_H
#define APPLICATION_H

#include <command.h>
#include <argument.h>
#include <image.h>

typedef struct {
    ArgumentList argument_list;
    CommandHandler command_handler;
    Image image;
    bool running:1,valid:1;
} Application;

API Application application_construct(int argc, char** argv);
API void        application_destruct(Application* self);
API void        application_perform_command(Application* self, CommandType type, void* payload);

#endif //APPLICATION_H
