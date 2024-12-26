#include <application.h>
#include <tests.h>

// TODO: Remove
#include <stdio.h>

int main(int argc, char** argv) {
    char* targv[] = {"Program-name", "--input", "input.tga", "--output", "24242.1654165"};

    EntryparamList epl = entryparamlist_constrcut(5, targv);

    entryparamlist_print(&epl);

    entryparamlist_destruct(&epl);

    return 0;
}
