/*
*  Ondrej Nedojedly
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/11/14
 */

#define ATTRIBUTE_CONSTRUCTOR __attribute__ ((constructor))
#define ATTRIBUTE_DECONSTRUCTOR __attribute__ ((destructor))

#ifndef __GNUC__
 #warning 'Untested compiler! Only GCC supported /GCC tested/; see https://github.com/ondranedo/photoshell'
#endif

#include <error.h>

static ErrorHandler error_handler;

void ATTRIBUTE_CONSTRUCTOR core_init() {
    error_handler = errorhandler_construct(4);
    errorhandler_set(&error_handler);
}

void ATTRIBUTE_DECONSTRUCTOR core_release() {
    errorhandler_destruct(&error_handler);
}