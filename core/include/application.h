/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <argument.h>
#include <error.h>

typedef struct {
    ArgumentList argument_list;
    bool running:1,valid:1;
} Application;

API Application application_construct(int argc, char** argv);
API void        application_destruct(Application* self);

#endif //APPLICATION_H
