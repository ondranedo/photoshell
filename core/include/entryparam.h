/*
*  Ondrej Nedojedly
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#ifndef ENTRYPARAMETERS_H
#define ENTRYPARAMETERS_H

#include <core.h>

typedef enum : u16 {
    //  xxxx xxxx xxxx xABB
    //  x - unused A - Required B - Number / String
    ENTRYPARAM_VOID     = 0x000,
    ENTRYPARAM_STRING   = 0x001,
    ENTRYPARAM_NUMBER   = 0x010,
    ENTRYPARAM_REQUIRED = 0x100,
   } EntryparamTraits;

typedef enum : u8 {
    ENTRYPARAM_INPUT,
    ENTRYPARAM_OUTPUT,
    ENTRYPARAM_VERSION,
    ENTRYPARAM_HELP,
    ENTRYPARAM_NIL
   } EntryparamType;

typedef struct {
    EntryparamType type;
    union {
        char*  string;  //TODO: Add string
        double number;
    } payload;
} Entryparam;

Entryparam entryparam_construct_string(EntryparamType type, const char* string);

Entryparam entryparam_construct_number(EntryparamType type, double number);

bool entryparam_isvalid_name(const char* name);

EntryparamType entryparam_get_type(const char* name);

EntryparamTraits entryparam_get_traits(const EntryparamType type);

typedef struct {
    Entryparam* list;
    u64 count;
} EntryparamList;


API EntryparamList entryparamlist_constrcut(int argc, char** argv);

API void entryparamlist_destruct(EntryparamList* self);

API void entryparamlist_print(EntryparamList* self);

#endif //ENTRYPARAMETERS_H