/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/11/14
 */

#ifndef ERRORS_H
#define ERRORS_H

#include <core.h>

typedef enum : u8 {
    ERROR_NONE = 0x00,
    ERROR_TEST = 0x01,
    ERROR_NIL
} ErrorCode;

typedef struct {
    ErrorCode code;
    char msg[32]; // It's OK, used for debugging purposes anyway;
    bool fatal;
} Error;

#endif//ERRORS_H
