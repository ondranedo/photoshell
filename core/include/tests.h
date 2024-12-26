/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/11/14
 */

#ifndef TESTS_H
#define TESTS_H

#ifdef ENABLE_TESTS

#include <errors.h>

extern Error _init_tests(); /* Tests executed right after entrypoint */

// Defines
    #define INIT_TESTS() _init_tests()
#else
    #define INIT_TESTS() {}
#endif

#endif //TESTS_H
