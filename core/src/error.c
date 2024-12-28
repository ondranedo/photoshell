/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/28
 */

#include <error.h>

// TODO: Remove
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static ErrorHandler* handler;

ErrorHandler errorhandler_construct(u128 default_max_count) {
    ErrorHandler handler = {
        .error_count = 0,
        .max_count = default_max_count,
        .errors = malloc(sizeof(Error)*default_max_count)
    };
    return handler;
}

void errorhandler_destruct(ErrorHandler * self) {
    free(self->errors);
}

void errorhandler_set(ErrorHandler* h) {
    if(!handler)
        handler = h;
}

void errorhandler_handle() {
    if(!handler) return;
    u128 i = 0;
    bool e = false;
    for(; i < handler->error_count; ++i) {
        fprintf(stderr, "[ERROR] %s\n", handler->errors[i].msg);
        e |= handler->errors[i].fatal;
        memset(handler->errors + i, 0x00, sizeof(Error));
    }
    handler->error_count = 0;
    if(e) exit(handler->errors[i].code);
}

static inline void _handler_resize() {
    if(!handler) return;
    handler->errors = realloc(handler->errors, handler->max_count*=2);
}

void error_throw(ErrorCode code, const char* msg, bool fatal) {
    if(!handler) return;
    if(handler->error_count >= handler->max_count)
        _handler_resize();

    handler->errors[handler->error_count].code = code;
    handler->errors[handler->error_count].fatal = fatal;
    strcpy(handler->errors[handler->error_count].msg,msg);
    handler->error_count++;
}
