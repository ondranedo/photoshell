/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/29
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <core.h>

typedef enum {
    COMMAND_NONE,
    COMMAND_RESIZE,
    COMMAND_CROP,
    COMMAND_COPY,
    COMMAND_MOVE,
    COMMAND_BW,
    COMMAND_INFO,
    COMMAND_SAVE,
    COMMAND_EXIT,
    COMMAND_EOL
} CommandType;

typedef struct { u128 w, h; } CommandResize;
typedef struct { u128 x, y, w, h; } CommandCrop;
typedef struct { u128 x1, x2, y1, y2, w, h; } CommandCopy;
typedef struct { u128 x, y; } CommandMove;

typedef void(*CommandFn)(void* application, void* data);

typedef struct {
    CommandType type;
    void* payload;
} Command;

typedef struct {
    CommandFn* function;
    u128 function_count;
} CommandHandler;

API Command   command_construct(CommandType type, void* payload);
API void      command_destruct(Command* self);

API CommandHandler commandhandler_construct();
API void           commandhandler_destruct(CommandHandler* self);
API void           commandhandler_set_execute_fn(CommandHandler* handler, CommandType type, CommandFn fn);
API CommandFn      commandhandler_get_execute_fn(CommandHandler* handler, CommandType type);

#endif //COMMAND_H
