/*
 *  Ondrej Nedojedly 
 *     https://github.com/ondranedo/photoshell
 *  Created: 2024/12/15
 */

#include <entryparam.h>


// TODO: Remove
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <base/string.h>

static const EntryparamTemplate* template_list = NULL;

const char* entryparamtype_to_string(EntryparamType type) {
    switch(type) {
        case ENTRYPARAM_INPUT: return "Input file";
        case ENTRYPARAM_OUTPUT: return "Output file";
        case ENTRYPARAM_VERSION: return "Program version";
        case ENTRYPARAM_HELP: return "Help";
        case ENTRYPARAM_NIL: return "End of list";
    }
    return "<missing_value>at[const char*entryparam_to_string(EntryparamType entry_param)]";
}

void entryparamtemplate_set(const EntryparamTemplate* template) {
    if(!template_list)
        template_list = template;
}

EntryparamTraits entryparamtemplate_type_to_traits(const EntryparamType type) {
    u128 i = 0;
    if(!template_list) return ENTRYPARAM_VOID;
    for(; template_list[i].type != ENTRYPARAM_NIL; ++i) {
        if(template_list[i].type == type)
            return template_list[i].traits;
    }

    return ENTRYPARAM_VOID;
}

u128 entryparamtemplate_count_traits(EntryparamTraits traits) {
    u128 i = 0, count = 0;
    if(!template_list) return ENTRYPARAM_VOID;
    for(; template_list[i].type != ENTRYPARAM_NIL; ++i)
        if(template_list[i].traits & traits) count++;
    return count;
}

u128 entryparamtemplate_get_specific_types(EntryparamType* type, u128 max_count, EntryparamTraits traits) {
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

EntryparamType entryparamtemplate_string_to_type(const char* name) {
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

Entryparam entryparam_construct_string(EntryparamType type, const char* string) {
    Entryparam ep = {.type = type};
    ep.payload.string = (char*)malloc(sizeof(char)*strlen(string) + 1);
    strcpy(ep.payload.string, string);
    return ep;
}

Entryparam entryparam_construct_number(EntryparamType type, double number) {
    Entryparam ep = {.type = type};
    ep.payload.number = number;
    return ep;
}

bool entryparam_isvalid_name(const char* name) {
    auto type = entryparamtemplate_string_to_type(name);
    return type != ENTRYPARAM_NIL;
}

void entryparam_destruct(Entryparam* self) {
    auto traits = entryparamtemplate_type_to_traits(self->type);
    if(traits & ENTRYPARAM_STRING)
        free(self->payload.string);
}

typedef struct {char* adr; u8 offset; EntryparamType type; } _ArgvString;
typedef struct {double num; EntryparamType type; } _ArgvNumber;
typedef EntryparamType _ArgvVoid;

inline char* _parse_argv_param(char* argv) {
    return argv + string_count_front(argv, '-');
}

inline EntryparamTraits _handle_new_param(EntryparamList* epl, EntryparamTraits* traits, bool* mode, char* raw) {
    if(*mode || !entryparam_isvalid_name(_parse_argv_param(raw))) return false;
    epl->list[epl->count].type = entryparamtemplate_string_to_type(_parse_argv_param(raw));
    *traits = entryparamtemplate_type_to_traits(epl->list[epl->count].type);
    if(*traits & ENTRYPARAM_NUMBER || *traits & ENTRYPARAM_STRING) *mode = true;
    else epl->count++;
    return true;
}

inline Entryparam _create_new_param(EntryparamType type, EntryparamTraits traits, char* raw) {
    // TODO:                                                                    / In case of error it leads to crash
    //                                                                          v
    return traits & ENTRYPARAM_NUMBER ? entryparam_construct_number(type, atof(raw)) :
           traits & ENTRYPARAM_STRING ? entryparam_construct_string(type, raw) :
           (Entryparam){type, NULL};
}

EntryparamList entryparamlist_construct(int argc, char** argv) {
    EntryparamList epl = { .list = malloc((argc - 1)*sizeof(Entryparam)), .count = 0 };
    bool input_mode = false;
    u128 i = 0;
    EntryparamTraits traits = ENTRYPARAM_VOID;

    for(;i < argc - 1; ++i) {
        if(_handle_new_param(&epl, &traits, &input_mode, argv[i + 1])) continue;
        if(input_mode) {
            epl.list[epl.count++] = _create_new_param(epl.list[epl.count].type, traits, argv[i + 1]);
            input_mode = false;
        }
    }

    return epl;
}

void entryparamlist_destruct(EntryparamList* self) {
    u128 i = 0;
    for(; i < self->count; ++i) {
        entryparam_destruct(self->list + i);
    }

    free(self->list);
}

void entryparamlist_print(EntryparamList* self) {
    u128 i = 0;
    for(;i < self->count; ++i) {
        if(entryparamtemplate_type_to_traits(self->list[i].type) & ENTRYPARAM_NUMBER)
            printf("(%llu) %lf\n", self->count, self->list[i].payload.number);
        else
        if(entryparamtemplate_type_to_traits(self->list[i].type) & ENTRYPARAM_STRING)
            printf("(%llu) %s\n", self->count, self->list[i].payload.string);
        else
            printf("(%llu) void\n", self->count);
    }
}

Error entryparamlist_validate(EntryparamList* list) {
    EntryparamType* required = (EntryparamType*)malloc(entryparamtemplate_count_traits(ENTRYPARAM_REQUIRED)*sizeof(EntryparamType));
    EntryparamType* unique = (EntryparamType*)malloc(entryparamtemplate_count_traits(ENTRYPARAM_UNIQUE)*sizeof(EntryparamType));
    u128 i = 0, j = 0, unique_count = 0;
    Error error = {ERROR_ARGC, "", true};
    bool validate_required = true;

    entryparamtemplate_get_specific_types(required, entryparamtemplate_count_traits(ENTRYPARAM_REQUIRED), ENTRYPARAM_REQUIRED);

    if(list->count == 0 && entryparamtemplate_count_traits(ENTRYPARAM_REQUIRED) > 0) {
        snprintf(error.msg, ERROR_MSG_MAX_SIZE, "Program missing input.");
        free(required);
        free(unique);
        return error;
    }

    for(;i < list->count; ++i) {
        // Validate Unique
        if(entryparamtemplate_type_to_traits(list->list[i].type) & ENTRYPARAM_UNIQUE) {
            for(j = 0; j < unique_count; ++j)
                if(unique[j] == list->list[i].type) {
                    snprintf(error.msg, ERROR_MSG_MAX_SIZE, "Validating UNIQUE argument trait with (%s).", entryparamtype_to_string(unique[j]));
                    free(required);
                    free(unique);
                    return error;
                }
            unique[unique_count++] = list->list[i].type;
        }

        // Valida Unambiguous
        if(entryparamtemplate_type_to_traits(list->list[i].type) & ENTRYPARAM_UNAMBIGUOUS) {
            if(list->count > 1) { // TODO: Set conflicting arguments instead of '> 1'
                snprintf(error.msg, ERROR_MSG_MAX_SIZE, "Validating UNAMBIGUOUS argument trait with (%s).", entryparamtype_to_string(list->list[i].type));
                free(required);
                free(unique);
                return error;
            }

            validate_required = false;
        }

        //Set for required
        for(j = 0; j < entryparamtemplate_count_traits(ENTRYPARAM_REQUIRED); ++j)
            if(list->list[i].type == required[j]) required[j] = ENTRYPARAM_VOID;
    }

    // Validate required
    if(validate_required)
    for(i = 0; i < entryparamtemplate_count_traits(ENTRYPARAM_REQUIRED); ++i) {
        if(required[i] == ENTRYPARAM_VOID) continue;

        snprintf(error.msg, ERROR_MSG_MAX_SIZE, "Validating REQUIRED argument trait with (%s).", entryparamtype_to_string(required[i]));
        free(required);
        free(unique);
        return error;
    }


    free(required);
    free(unique);

    return (Error){ ERROR_NONE, "", false};
}