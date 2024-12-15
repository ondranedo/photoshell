/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#include <entryparam.h>

static const Entryparam entryparam_list[] = {
    {"version", 'v', ENTRYPARAM_VOID, 0},
    {"help", 'h', ENTRYPARAM_VOID,  0},
    {"input", 'i', ENTRYPARAM_STRING | ENTRYPARAM_REQUIRED, 0},
    {"output", 'o', ENTRYPARAM_STRING | ENTRYPARAM_REQUIRED, 0},
};

EntryparamList entryparamlist_constrcut(int argc, char** argv) {
    EntryparamList list = {0};

    return list;
}

void entryparamlist_destruct(EntryparamList* self) {

}
