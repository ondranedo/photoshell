/*
*  Ondrej Nedojedly
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#ifndef ENTRYPARAM_H
#define ENTRYPARAM_H

#include <core.h>
#include <error.h>

typedef enum : u8 {
    // xxxCUABB(u8)
    ENTRYPARAM_VOID        = 0b00000000,
    ENTRYPARAM_STRING      = 0b00000001, // B
    ENTRYPARAM_NUMBER      = 0b00000010, // B
    ENTRYPARAM_REQUIRED    = 0b00000100, // A
    ENTRYPARAM_UNIQUE      = 0b00001000, // U
    ENTRYPARAM_UNAMBIGUOUS = 0b00010000, // C
} ArgumentTraits;

typedef enum : u8 {
    ENTRYPARAM_INPUT,    // Sets image input path
    ENTRYPARAM_OUTPUT,   // Sets image output path
    ENTRYPARAM_VERSION,  // Prints version
    ENTRYPARAM_HELP,     // Prints help
    ENTRYPARAM_NIL       // Do not use
} ArgumentType;

const char* argumenttype_to_string(ArgumentType entry_param);

typedef struct {
    const char* long_name;
    const char short_name;
    const ArgumentType type;
    const ArgumentTraits traits;
} ArgumentTemplate;

API void argumenttemplate_set(const ArgumentTemplate* template);

ArgumentType argumenttemplate_string_to_type(const char* name);

ArgumentTraits argumenttemplate_type_to_traits(const ArgumentType type);

u128 argumenttemplate_count_traits(ArgumentTraits traits);

u128 argumenttemplate_get_specific_types(ArgumentType* type, u128 max_count, ArgumentTraits traits);

typedef struct {
    ArgumentType type;
    union {
        char*  string;  //TODO: Add string
        double number;
    } payload;
} Argument;

Argument argument_construct_string(ArgumentType type, const char* string);

Argument argument_construct_number(ArgumentType type, double number);

bool argument_validate(const char* name);

typedef struct {
    Argument* list;
    u64 count;
} ArgumentList;

API ArgumentList argumentlist_construct(int argc, char** argv);

API void argumentlist_destruct(ArgumentList* self);

API void argumentlist_print(ArgumentList* self);

API void argumentlist_validate(ArgumentList* list);

#endif //ENTRYPARAM_H