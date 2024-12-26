/*
*  Ondrej Nedojedly
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/11/14
 */

#include "tests.h"

#ifdef PLATFORM_LINUX

#include <errors.h>
#include <stdio.h>
#include <application.h>

int main(int argc, char** argv) {
    const Error e = INIT_TESTS();

    if(e.code != ERROR_NONE) {
        fprintf(stderr, "Error detected: %s", e.msg);
        if(e.fatal) return e.code;
    }

    EntryparamList epl = entryparamlist_constrcut(argc, argv);
    Application app = application_construct(&epl);



    application_destruct(&app);
    entryparamlist_destruct(&epl);

    return 0;
}

#else

#error "Undefined platform! Only Linux supported /ARCH tested/; see https://github.com/ondranedo/photoshell"

#endif