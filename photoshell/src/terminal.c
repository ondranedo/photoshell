#include <application.h>
#include <tests.h>

// TODO: Remove
#include <stdio.h>

EntryparamTemplate template[] = {
    {"version", 'v', ENTRYPARAM_VERSION, ENTRYPARAM_VOID | ENTRYPARAM_UNAMBIGUOUS},
    {"help", 'h', ENTRYPARAM_HELP,ENTRYPARAM_VOID | ENTRYPARAM_UNAMBIGUOUS},
    {"input", 'i', ENTRYPARAM_INPUT, ENTRYPARAM_STRING | ENTRYPARAM_REQUIRED | ENTRYPARAM_UNIQUE },
    {"output", 'o', ENTRYPARAM_OUTPUT,ENTRYPARAM_STRING | ENTRYPARAM_REQUIRED | ENTRYPARAM_UNIQUE },
    {.type = ENTRYPARAM_NIL}
};

int main(int argc, char** argv) {
    char* targv[] = {"Program-name",
        "-i", "path.tga",
        "--output", "out.tga",
        "#"};
    int targc = 0; while(targv[targc++][0] != '#');

    entryparamtemplate_set(template);
    EntryparamList epl = entryparamlist_construct(targc - 1, targv);

    Error error = entryparamlist_validate(&epl);

    if(error.code != ERROR_NONE) {
        printf("Error: %s\n", error.msg);
        if(error.fatal)
            goto END;
    }

    entryparamlist_print(&epl);

END:
    entryparamlist_destruct(&epl);
    return 0;
}