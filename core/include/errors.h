/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/11/14
 */

#ifndef ERRORS_H
#define ERRORS_H

#include <core.h>

#define ERROR_MSG_MAX_SIZE 256

typedef enum : u8 {
    ERROR_NONE,
    ERROR_TEST,
    ERROR_ARGC,
    ERROR_NIL
} ErrorCode;

typedef struct {
    ErrorCode code;
    char msg[ERROR_MSG_MAX_SIZE];  // It's OK, used for debugging purposes anyway;
    bool fatal;
} Error;

#endif//ERRORS_H
