#include <application.h>

ArgumentTemplate template[] = {
    {"version", 'v', ARGUMENT_VERSION, ARGUMENT_TRAIT_VOID | ARGUMENT_TRAIT_UNAMBIGUOUS},
    {"help", 'h', ARGUMENT_HELP,ARGUMENT_TRAIT_VOID | ARGUMENT_TRAIT_UNAMBIGUOUS},
    {"input", 'i', ARGUMENT_INPUT, ARGUMENT_TRAIT_STRING | ARGUMENT_TRAIT_REQUIRED | ARGUMENT_TRAIT_UNIQUE },
    {"output", 'o', ARGUMENT_OUTPUT,ARGUMENT_TRAIT_STRING | ARGUMENT_TRAIT_REQUIRED | ARGUMENT_TRAIT_UNIQUE },
    {.type = ARGUMENT_EOL}
};

int main(int argc, char** argv) {
    char* targv[] = {"Program-name",
        "-i", "input.tga",
        "-o", "output.tga",
        "#"};
    int targc = 0; while(targv[targc++][0] != '#');

    argument_set_template(template);

    Application app = application_construct(targc - 1, targv);

    application_destruct(&app);

    return 0;
}