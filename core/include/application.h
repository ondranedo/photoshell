/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <entryparam.h>

typedef struct {
    void* data;
} Application;

API Application application_construct(EntryparamList* ep);

API void application_destruct(Application* self);

#endif //APPLICATION_H
