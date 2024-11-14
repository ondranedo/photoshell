/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/11/14
 */

#ifndef TESTS_H
#define TESTS_H

#ifdef PS_TESTS

#include <errors.h>

extern Error _run_init_tests(); /* Tests executed right after entrypoint */

// Defines
    #define PS_INIT_TESTS() _run_init_tests()
#else
    #define PS_INIT_TESTS() {}
#endif

#endif //TESTS_H
