/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <arguments.h>

typedef struct {
    void* data;
} Application;

extern Application application_construct(ArgumentList* ep);

extern void application_destruct(Application* self);

#endif //APPLICATION_H
