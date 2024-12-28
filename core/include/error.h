/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/11/14
 */

#ifndef ERRORS_H
#define ERRORS_H

#include <core.h>

#define ERROR_MSG_MAX_SIZE 128

typedef enum : u8 {
    ERROR_NONE,
    ERROR_TEST,
    ERROR_ARGC,
    ERROR_NIL
} ErrorCode;

typedef struct {
    ErrorCode code;
    char msg[ERROR_MSG_MAX_SIZE];
    bool fatal;
} Error;

typedef struct {
    Error* errors;
    u128 error_count, max_count;
} ErrorHandler;

API ErrorHandler errorhandler_construct(u128 default_max_count);

API void errorhandler_destruct(ErrorHandler* self);

API void errorhandler_set(ErrorHandler* handler);

API void errorhandler_handle();

API void error_throw(ErrorCode code, const char* msg, bool fatal);

#endif//ERRORS_H
