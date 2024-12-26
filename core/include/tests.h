/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/11/14
 */

#ifndef TESTS_H
#define TESTS_H

#include <errors.h>

API Error _init_tests(); /* Tests executed right after entrypoint */

#ifdef ENABLE_TESTS
    #define INIT_TESTS() _init_tests()
#else
    #define INIT_TESTS() {}
#endif

#endif //TESTS_H
