/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#include <arguments.h>


// TODO: Remove
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <base/string.h>

static const ArgumentTemplate* template_list = NULL;

const char* argumenttype_to_string(ArgumentType type) {
    switch(type) {
        case ENTRYPARAM_INPUT: return "Input file";
        case ENTRYPARAM_OUTPUT: return "Output file";
        case ENTRYPARAM_VERSION: return "Program version";
        case ENTRYPARAM_HELP: return "Help";
        case ENTRYPARAM_NIL: return "End of list";
    }
    return "<missing_value>at[const char*entryparam_to_string(EntryparamType entry_param)]";
}

void argumenttemplate_set(const ArgumentTemplate* template) {
    if(!template_list)
        template_list = template;
}

ArgumentTraits argumenttemplate_type_to_traits(const ArgumentType type) {
    u128 i = 0;
    if(!template_list) return ENTRYPARAM_VOID;
    for(; template_list[i].type != ENTRYPARAM_NIL; ++i) {
        if(template_list[i].type == type)
            return template_list[i].traits;
    }

    return ENTRYPARAM_VOID;
}

u128 argumenttemplate_count_traits(ArgumentTraits traits) {
    u128 i = 0, count = 0;
    if(!template_list) return ENTRYPARAM_VOID;
    for(; template_list[i].type != ENTRYPARAM_NIL; ++i)
        if(template_list[i].traits & traits) count++;
    return count;
}

u128 argumenttemplate_get_specific_types(ArgumentType* type, u128 max_count, ArgumentTraits traits) {
    u128 i = 0, count = 0;
    if(!template_list) return ENTRYPARAM_VOID;
    for(; template_list[i].type != ENTRYPARAM_NIL; ++i) {
        if(template_list[i].traits & traits) {
            type[count] = template_list[i].type;
            if(count < max_count) count++;
            else return count; // TODO: Throw some error or smth.
        }
    }

    return count;
}

ArgumentType argumenttemplate_string_to_type(const char* name) {
    u128 i = 0;
    const u128 il = (u128)strlen(name);
    if(!template_list) return ENTRYPARAM_NIL;

    for(; template_list[i].type != ENTRYPARAM_NIL; ++i) {
        if(!strcmp(template_list[i].long_name, name))
            return template_list[i].type;
        if(il==1 && *name == template_list[i].short_name)
            return template_list[i].type;
    }
    return ENTRYPARAM_NIL;
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
    auto type = argumenttemplate_string_to_type(name);
    return type != ENTRYPARAM_NIL;
}

void entryparam_destruct(Argument* self) {
    auto traits = argumenttemplate_type_to_traits(self->type);
    if(traits & ENTRYPARAM_STRING)
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
    epl->list[epl->count].type = argumenttemplate_string_to_type(_parse_argv_param(raw));
    *traits = argumenttemplate_type_to_traits(epl->list[epl->count].type);
    if(*traits & ENTRYPARAM_NUMBER || *traits & ENTRYPARAM_STRING) *mode = true;
    else epl->count++;
    return true;
}

inline Argument _create_new_param(ArgumentType type, ArgumentTraits traits, char* raw) {
    // TODO:                                                                    / In case of error it leads to crash
    //                                                                          v
    return traits & ENTRYPARAM_NUMBER ? argument_construct_number(type, atof(raw)) :
           traits & ENTRYPARAM_STRING ? argument_construct_string(type, raw) :
           (Argument){type, NULL};
}

ArgumentList argumentlist_construct(int argc, char** argv) {
    ArgumentList epl = { .list = malloc((argc - 1)*sizeof(Argument)), .count = 0 };
    bool input_mode = false;
    u128 i = 0;
    ArgumentTraits traits = ENTRYPARAM_VOID;

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
        entryparam_destruct(self->list + i);
    }

    free(self->list);
}

void argumentlist_print(ArgumentList* self) {
    u128 i = 0;
    for(;i < self->count; ++i) {
        if(argumenttemplate_type_to_traits(self->list[i].type) & ENTRYPARAM_NUMBER)
            printf("(%s) %lf\n", argumenttype_to_string(self->list[i].type), self->list[i].payload.number);
        else
        if(argumenttemplate_type_to_traits(self->list[i].type) & ENTRYPARAM_STRING)
            printf("(%s) %s\n", argumenttype_to_string(self->list[i].type), self->list[i].payload.string);
        else
            printf("(%s)\n", argumenttype_to_string(self->list[i].type));
    }
}

static inline Error _validate_required(ArgumentList* list, ArgumentType* required) {
    Error error = {ERROR_NONE, "", false };
    u128 i = 0, j = 0;
    if(list->count == 0 && argumenttemplate_count_traits(ENTRYPARAM_REQUIRED) > 0) {
        //TODO Error logger.
        snprintf(error.msg, ERROR_MSG_MAX_SIZE, "Program missing input.");
        error.fatal = true;
        error.code = ERROR_ARGC;
        return error;
    }

    for(;i < list->count; ++i) {
        for(j = 0; j < argumenttemplate_count_traits(ENTRYPARAM_REQUIRED); ++j)
            if(list->list[i].type == required[j]) required[j] = ENTRYPARAM_VOID;
    }

    for(i = 0; i < argumenttemplate_count_traits(ENTRYPARAM_REQUIRED); ++i) {
        if(required[i] == ENTRYPARAM_VOID) continue;
        //TODO Error logger.
        snprintf(error.msg, ERROR_MSG_MAX_SIZE, "REQUIRED argument trait violated with argument \'%s\'.", argumenttype_to_string(required[i]));
        error.fatal = true;
        error.code = ERROR_ARGC;
        return error;
    }

    return error;
}

static inline Error _validate_unique(ArgumentList* list, ArgumentType* unique) {
    Error error = {ERROR_NONE, "", false };
    u128 i = 0, j = 0, unique_count = 0;

    for(;i < list->count; ++i) {
        // Validate Unique
        if(argumenttemplate_type_to_traits(list->list[i].type) & ENTRYPARAM_UNIQUE) {
            for(j = 0; j < unique_count; ++j)
                if(unique[j] == list->list[i].type) {
                    //TODO Error logger.
                    snprintf(error.msg, ERROR_MSG_MAX_SIZE, "UNIQUE argument trait violated with argument \'%s\'.", argumenttype_to_string(unique[j]));
                    error.fatal = true;
                    error.code = ERROR_ARGC;
                    return error;
                }
            unique[unique_count++] = list->list[i].type;
        }
    }

    return error;
}

static inline Error _validate_unambiguous(ArgumentList* list, bool* validate_required) {
    Error error = { ERROR_NONE, "", false };
    u128 i = 0;
    for(;i < list->count; ++i) {
        // Valida Unambiguous
        if(argumenttemplate_type_to_traits(list->list[i].type) & ENTRYPARAM_UNAMBIGUOUS) {
            if(list->count > 1) { // TODO: Set conflicting arguments instead of '> 1'
                //TODO Error logger.
                snprintf(error.msg, ERROR_MSG_MAX_SIZE, "UNAMBIGUOUS argument trait violated with argument \'%s\'.", argumenttype_to_string(list->list[i].type));
                error.code = ERROR_ARGC;
                error.fatal = true;
                return error;
            }

            *validate_required = false;
        }
    }
    return error;
}

Error argumentlist_validate(ArgumentList* list) {
    ArgumentType* required = (ArgumentType*)malloc(argumenttemplate_count_traits(ENTRYPARAM_REQUIRED)*sizeof(ArgumentType));
    ArgumentType* unique = (ArgumentType*)malloc(argumenttemplate_count_traits(ENTRYPARAM_UNIQUE)*sizeof(ArgumentType));
    u128 i = 0, j = 0;
    Error error = {ERROR_NONE, "", false };
    bool validate_required = true;

    argumenttemplate_get_specific_types(required, argumenttemplate_count_traits(ENTRYPARAM_REQUIRED), ENTRYPARAM_REQUIRED);

    error = _validate_unique(list, unique);
    if(error.code == ERROR_NONE)
        error = _validate_unambiguous(list, &validate_required);
    if(validate_required && error.code == ERROR_NONE)
        error = _validate_required(list, required);

    free(required);
    free(unique);

    return error;
}