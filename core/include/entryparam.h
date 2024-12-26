/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#ifndef ENTRYPARAMETERS_H
#define ENTRYPARAMETERS_H

#include <core.h>

typedef enum {//unused(5b)|required(1b)|type(2b)
    ENTRYPARAM_VOID     = 0x000,
    ENTRYPARAM_STRING   = 0x001,
    ENTRYPARAM_NUMBER   = 0x010,
    ENTRYPARAM_REQUIRED = 0x100,
} EntryparamType;

typedef union {
    char*  string;
    double number;
} EntryPayload;

typedef struct {
    const char* lvar;
    const char svar;
    EntryparamType type;
    EntryPayload payload;
} Entryparam;

typedef struct {
    Entryparam* ep;
    u64 count;
} EntryparamList;

EntryparamList entryparamlist_constrcut(int argc, char** argv);

void entryparamlist_destruct(EntryparamList* self);

#endif //ENTRYPARAMETERS_H
