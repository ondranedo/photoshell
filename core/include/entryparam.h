/*
*  Ondrej Nedojedly
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#ifndef ENTRYPARAM_H
#define ENTRYPARAM_H

#include <core.h>
#include <errors.h>

typedef enum : u8 {
    // xxxCUABB(u8)
    ENTRYPARAM_VOID        = 0b00000000,
    ENTRYPARAM_STRING      = 0b00000001, // B
    ENTRYPARAM_NUMBER      = 0b00000010, // B
    ENTRYPARAM_REQUIRED    = 0b00000100, // A
    ENTRYPARAM_UNIQUE      = 0b00001000, // U
    ENTRYPARAM_UNAMBIGUOUS = 0b00010000, // C
} EntryparamTraits;

typedef enum : u8 {
    ENTRYPARAM_INPUT,    // Sets image input path
    ENTRYPARAM_OUTPUT,   // Sets image output path
    ENTRYPARAM_VERSION,  // Prints version
    ENTRYPARAM_HELP,     // Prints help
    ENTRYPARAM_NIL       // Do not use
   } EntryparamType;

const char* entryparamtype_to_string(EntryparamType entry_param);

typedef struct {
    const char* long_name;
    const char short_name;
    const EntryparamType type;
    const EntryparamTraits traits;
} EntryparamTemplate;

API void entryparamtemplate_set(const EntryparamTemplate* template);

EntryparamType entryparamtemplate_string_to_type(const char* name);

EntryparamTraits entryparamtemplate_type_to_traits(const EntryparamType type);

u128 entryparamtemplate_count_traits(EntryparamTraits traits);

u128 entryparamtemplate_get_specific_types(EntryparamType* type, u128 max_count, EntryparamTraits traits);

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

typedef struct {
    Entryparam* list;
    u64 count;
} EntryparamList;

API EntryparamList entryparamlist_construct(int argc, char** argv);

API void entryparamlist_destruct(EntryparamList* self);

API void entryparamlist_print(EntryparamList* self);

API Error entryparamlist_validate(EntryparamList* list);

#endif //ENTRYPARAM_H