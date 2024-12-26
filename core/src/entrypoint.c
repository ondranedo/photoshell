/*
*  Ondrej Nedojedly
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/11/14
 */

#ifdef __GNUC__

#define ATTRIBUTE_CONSTRUCTOR __attribute__ ((constructor))
#define ATTRIBUTE_DECONSTRUCTOR __attribute__ ((destructor))

#else
 #define ATTRIBUTE_CONSTRUCTOR __attribute__ ((constructor))
 #define ATTRIBUTE_DECONSTRUCTOR  __attribute__ ((destructor))

 #warning 'Untested compiler! Only GCC supported /GCC tested/; see https://github.com/ondranedo/photoshell'
#endif

#include <tests.h>
#include <stdio.h>
#include <stdlib.h>

void ATTRIBUTE_CONSTRUCTOR core_init() {
    const Error e = INIT_TESTS();

    if(e.code != ERROR_NONE) {
        //TODO: Replace with internal logger
        printf("[CORE INIT] [ERROR]: %s\n", e.msg);
        if(e.fatal) exit(e.code);
    }
}

void ATTRIBUTE_DECONSTRUCTOR core_release() {

}





