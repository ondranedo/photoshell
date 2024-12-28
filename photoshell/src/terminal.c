#include <application.h>
#include <tests.h>

// TODO: Remove
#include <stdio.h>

ArgumentTemplate template[] = {
    {"version", 'v', ENTRYPARAM_VERSION, ENTRYPARAM_VOID | ENTRYPARAM_UNAMBIGUOUS},
    {"help", 'h', ENTRYPARAM_HELP,ENTRYPARAM_VOID | ENTRYPARAM_UNAMBIGUOUS},
    {"input", 'i', ENTRYPARAM_INPUT, ENTRYPARAM_STRING | ENTRYPARAM_REQUIRED | ENTRYPARAM_UNIQUE },
    {"output", 'o', ENTRYPARAM_OUTPUT,ENTRYPARAM_STRING | ENTRYPARAM_REQUIRED | ENTRYPARAM_UNIQUE },
    {.type = ENTRYPARAM_NIL}
};

int main(int argc, char** argv) {
    char* targv[] = {"Program-name",
        "-i", "input.tga",
        "-o", "output.tga",
        "#"};
    int targc = 0; while(targv[targc++][0] != '#');

    argumenttemplate_set(template);
    ArgumentList al = argumentlist_construct(targc - 1, targv);

    Error error = argumentlist_validate(&al);

    if(error.code != ERROR_NONE) {
        printf("Error: %s\n", error.msg);
        if(error.fatal) {
            argumentlist_destruct(&al);
            return 1;
        }
    }

    argumentlist_print(&al);

    argumentlist_destruct(&al);

    return 0;
}