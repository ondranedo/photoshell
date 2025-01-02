/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/29
 */

#include <command.h>
#include <stdlib.h>

Command command_construct(CommandType type, void* payload) {
    return (Command){.payload = payload, .type = type};
}

void command_destruct(Command* self) {
    if(self->payload)
        free(self->payload);
}

void command_set_execute_fn(CommandType type, CommandFn fn) {

}

CommandHandler commandhandler_construct() {
    CommandHandler handler = {0};
    while(COMMAND_NONE + handler.function_count++ != COMMAND_EOL);
    handler.function_count-=1;
    handler.function = malloc(sizeof(CommandFn)*handler.function_count);
    return handler;
}

void commandhandler_destruct(CommandHandler* self) {
    free(self->function);
}

void commandhandler_set_execute_fn(CommandHandler* handler, CommandType type, CommandFn fn) {
    handler->function[type - 1] = fn;
}

CommandFn commandhandler_get_execute_fn(CommandHandler* handler, CommandType type) {
    return handler->function[type - 1];
}
