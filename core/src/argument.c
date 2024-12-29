/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#include <argument.h>
#include <error.h>

// TODO: Remove
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <base/string.h>

static const ArgumentTemplate* template_list = NULL;

const char* argument_type_to_string(ArgumentType type) {
    switch(type) {
        case ARGUMENT_INPUT: return "Input file";
        case ARGUMENT_OUTPUT: return "Output file";
        case ARGUMENT_VERSION: return "Program version";
        case ARGUMENT_HELP: return "Help";
        case ARGUMENT_EOL: return "End of list";
    }
    error_throw(ERROR_RUNT, "Unknown ArgumentType", false);
    return "<missing_value>";
}

void argument_set_template(const ArgumentTemplate* template) {
    if(!template_list)
        template_list = template;
}

ArgumentTraits argument_type_to_traits(const ArgumentType type) {
    u128 i = 0;
    if(!template_list) return ARGUMENT_TRAIT_VOID;
    for(; template_list[i].type != ARGUMENT_EOL; ++i) {
        if(template_list[i].type == type)
            return template_list[i].traits;
    }

    return ARGUMENT_TRAIT_VOID;
}

u128 argumenttemplate_count_traits(ArgumentTraits traits) {
    u128 i = 0, count = 0;
    if(!template_list) return ARGUMENT_TRAIT_VOID;
    for(; template_list[i].type != ARGUMENT_EOL; ++i)
        if(template_list[i].traits & traits) count++;
    return count;
}

u128 argumenttemplate_get_specific_types(ArgumentType* type, u128 max_count, ArgumentTraits traits) {
    u128 i = 0, count = 0;
    if(!template_list) return ARGUMENT_TRAIT_VOID;
    for(; template_list[i].type != ARGUMENT_EOL; ++i) {
        if(template_list[i].traits & traits) {
            type[count] = template_list[i].type;
            if(count < max_count) count++;
            else return count; // TODO: Throw some error or smth.
        }
    }

    return count;
}

ArgumentType argument_string_to_type(const char* name) {
    u128 i = 0;
    const u128 il = (u128)strlen(name);
    if(!template_list) return ARGUMENT_EOL;

    for(; template_list[i].type != ARGUMENT_EOL; ++i) {
        if(!strcmp(template_list[i].long_name, name))
            return template_list[i].type;
        if(il==1 && *name == template_list[i].short_name)
            return template_list[i].type;
    }
    return ARGUMENT_EOL;
}

Argument argument_construct_string(ArgumentType type, const char* string) {
    Argument ep = {.type = type};
    ep.payload.string = (char*)malloc(sizeof(char)*strlen(string) + 1);
    strcpy(ep.payload.string, string);
    return ep;
}

Argument argument_construct_number(ArgumentType type, double number) {
    Argument ep = {.type = type};
    ep.payload.number = number;
    return ep;
}

bool argument_validate(const char* name) {
    auto type = argument_string_to_type(name);
    return type != ARGUMENT_EOL;
}

void argument_destruct(Argument* self) {
    auto traits = argument_type_to_traits(self->type);
    if(traits & ARGUMENT_TRAIT_STRING)
        free(self->payload.string);
}

typedef struct {char* adr; u8 offset; ArgumentType type; } _ArgvString;
typedef struct {double num; ArgumentType type; } _ArgvNumber;
typedef ArgumentType _ArgvVoid;

inline char* _parse_argv_param(char* argv) {
    return argv + string_count_front(argv, '-');
}

inline ArgumentTraits _handle_new_param(ArgumentList* epl, ArgumentTraits* traits, bool* mode, char* raw) {
    if(*mode || !argument_validate(_parse_argv_param(raw))) return false;
    epl->list[epl->count].type = argument_string_to_type(_parse_argv_param(raw));
    *traits = argument_type_to_traits(epl->list[epl->count].type);
    if(*traits & ARGUMENT_TRAIT_NUMBER || *traits & ARGUMENT_TRAIT_STRING) *mode = true;
    else epl->count++;
    return true;
}

inline Argument _create_new_param(ArgumentType type, ArgumentTraits traits, char* raw) {
    // TODO:                                                                    / In case of error it leads to crash
    //                                                                          v
    return traits & ARGUMENT_TRAIT_NUMBER ? argument_construct_number(type, atof(raw)) :
           traits & ARGUMENT_TRAIT_STRING ? argument_construct_string(type, raw) :
           (Argument){type, NULL};
}

ArgumentList argumentlist_construct(int argc, char** argv) {
    ArgumentList epl = { .list = malloc((argc - 1)*sizeof(Argument)), .count = 0 };
    bool input_mode = false;
    u128 i = 0;
    ArgumentTraits traits = ARGUMENT_TRAIT_VOID;

    for(;i < argc - 1; ++i) {
        if(_handle_new_param(&epl, &traits, &input_mode, argv[i + 1])) continue;
        if(input_mode) {
            epl.list[epl.count++] = _create_new_param(epl.list[epl.count].type, traits, argv[i + 1]);
            input_mode = false;
        }
    }

    return epl;
}

void argumentlist_destruct(ArgumentList* self) {
    u128 i = 0;
    for(; i < self->count; ++i) {
        argument_destruct(self->list + i);
    }

    free(self->list);
}

void argumentlist_print(ArgumentList* self) {
    u128 i = 0;
    for(;i < self->count; ++i) {
        if(argument_type_to_traits(self->list[i].type) & ARGUMENT_TRAIT_NUMBER)
            //TODO: Add Logger
            printf("(%s) %lf\n", argument_type_to_string(self->list[i].type), self->list[i].payload.number);
        else
        if(argument_type_to_traits(self->list[i].type) & ARGUMENT_TRAIT_STRING)
            //TODO: Add Logger
            printf("(%s) %s\n", argument_type_to_string(self->list[i].type), self->list[i].payload.string);
        else
            //TODO: Add Logger
            printf("(%s)\n", argument_type_to_string(self->list[i].type));
    }
}

static inline void _validate_required(ArgumentList* list, ArgumentType* required) {
    u128 i = 0, j = 0;
    char buff[ERROR_MSG_MAX_SIZE];

    if(list->count == 0 && argumenttemplate_count_traits(ARGUMENT_TRAIT_REQUIRED) > 0) {
        snprintf(buff, ERROR_MSG_MAX_SIZE, "Program missing input.");
        error_throw(ERROR_ARGC, buff, true);
    }

    for(;i < list->count; ++i) {
        for(j = 0; j < argumenttemplate_count_traits(ARGUMENT_TRAIT_REQUIRED); ++j)
            if(list->list[i].type == required[j]) required[j] = ARGUMENT_TRAIT_VOID;
    }

    for(i = 0; i < argumenttemplate_count_traits(ARGUMENT_TRAIT_REQUIRED); ++i) {
        if(required[i] == ARGUMENT_TRAIT_VOID) continue;
        snprintf(buff, ERROR_MSG_MAX_SIZE, "REQUIRED argument trait violated with argument \'%s\'.", argument_type_to_string(required[i]));
        error_throw(ERROR_ARGC, buff, true);
    }
}

static inline void _validate_unique(ArgumentList* list, ArgumentType* unique) {
    u128 i = 0, j = 0, unique_count = 0;
    char buff[ERROR_MSG_MAX_SIZE];
    for(;i < list->count; ++i) {
        // Validate Unique
        if(argument_type_to_traits(list->list[i].type) & ARGUMENT_TRAIT_UNIQUE) {
            for(j = 0; j < unique_count; ++j)
                if(unique[j] == list->list[i].type) {
                    snprintf(buff, ERROR_MSG_MAX_SIZE, "UNIQUE argument trait violated with argument \'%s\'.", argument_type_to_string(unique[j]));
                    error_throw(ERROR_ARGC, buff, true);
                }
            unique[unique_count++] = list->list[i].type;
        }
    }
}

static inline void _validate_unambiguous(ArgumentList* list, bool* validate_required) {
    u128 i = 0;
    char buff[ERROR_MSG_MAX_SIZE];
    for(;i < list->count; ++i) {
        // Valida Unambiguous
        if(argument_type_to_traits(list->list[i].type) & ARGUMENT_TRAIT_UNAMBIGUOUS) {
            if(list->count > 1) { // TODO: Set conflicting arguments instead of '> 1'
                snprintf(buff, ERROR_MSG_MAX_SIZE, "UNAMBIGUOUS argument trait violated with argument \'%s\'.", argument_type_to_string(list->list[i].type));
                error_throw(ERROR_ARGC, buff, true);
            }
            *validate_required = false;
        }
    }
}

void argumentlist_validate(ArgumentList* list) {
    ArgumentType* required = (ArgumentType*)malloc(argumenttemplate_count_traits(ARGUMENT_TRAIT_REQUIRED)*sizeof(ArgumentType));
    ArgumentType* unique = (ArgumentType*)malloc(argumenttemplate_count_traits(ARGUMENT_TRAIT_UNIQUE)*sizeof(ArgumentType));
    u128 i = 0, j = 0;
    bool validate_required = true;

    argumenttemplate_get_specific_types(required, argumenttemplate_count_traits(ARGUMENT_TRAIT_REQUIRED), ARGUMENT_TRAIT_REQUIRED);

    _validate_unique(list, unique);

    _validate_unambiguous(list, &validate_required);
    if(validate_required)
       _validate_required(list, required);

    free(required);
    free(unique);
}