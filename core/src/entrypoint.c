/*
*  Ondrej Nedojedly
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/11/14
 */

#include "tests.h"

#ifdef PS_LINUX

#include <errors.h>
#include <tests.h>
#include <stdio.h>

int main(int argc, char** argv){

    const Error e = PS_INIT_TESTS();

    if(e.code != ERROR_NONE) {
        fprintf(stderr, "Error detected: %s", e.msg);
    }

    return 0;
}

#else

#error "Undefined platform! Only Linux supported /ARCH tested/; see https://github.com/ondranedo/photoshell"

#endif