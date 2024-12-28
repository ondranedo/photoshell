/*
*  Ondrej Nedojedly
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#ifndef ENTRYPARAM_H
#define ENTRYPARAM_H

#include <core.h>

typedef enum : u8 {
    // xxxCUABB(u8)
    ARGUMENT_TRAIT_VOID        = 0b00000000,
    ARGUMENT_TRAIT_STRING      = 0b00000001, // B
    ARGUMENT_TRAIT_NUMBER      = 0b00000010, // B
    ARGUMENT_TRAIT_REQUIRED    = 0b00000100, // A
    ARGUMENT_TRAIT_UNIQUE      = 0b00001000, // U
    ARGUMENT_TRAIT_UNAMBIGUOUS = 0b00010000, // C
} ArgumentTraits;

typedef enum : u8 {
    ARGUMENT_INPUT,    // Sets image input path
    ARGUMENT_OUTPUT,   // Sets image output path
    ARGUMENT_VERSION,  // Prints version
    ARGUMENT_HELP,     // Prints help
    ARGUMENT_EOL       // End of list
} ArgumentType;

typedef struct {
    const char* long_name;
    const char short_name;
    const ArgumentType type;
    const ArgumentTraits traits;
} ArgumentTemplate;

typedef struct {
    ArgumentType type;
    union {
        char*  string;  //TODO: Add 'string'
        double number;
    } payload;
} Argument;

typedef struct {
    Argument* list;
    u64 count;
} ArgumentList;

API Argument argument_construct_string(ArgumentType type, const char* string);
API Argument argument_construct_number(ArgumentType type, double number);
API void     argument_destruct(Argument* self);

API ArgumentType   argument_string_to_type(const char* name);
API ArgumentTraits argument_type_to_traits(const ArgumentType type);
API const char*    argument_type_to_string(ArgumentType entry_param);

API bool argument_validate(const char* name);
API void argument_set_template(const ArgumentTemplate* template);

API void         argumentlist_print(ArgumentList* self);
API ArgumentList argumentlist_construct(int argc, char** argv);
API void         argumentlist_validate(ArgumentList* list);
API void         argumentlist_destruct(ArgumentList* self);

API u128 argumenttemplate_count_traits(ArgumentTraits traits);
API u128 argumenttemplate_get_specific_types(ArgumentType* type, u128 max_count, ArgumentTraits traits);

#endif //ENTRYPARAM_H