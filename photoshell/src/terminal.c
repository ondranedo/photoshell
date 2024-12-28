#include <application.h>

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
    
    Application app = application_construct(targc - 1, targv);

    application_run(&app);

    application_destruct(&app);

    return 0;
}