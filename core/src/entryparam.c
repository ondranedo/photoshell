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

typedef struct {
    const char* long_name;
    const char short_name;
    const EntryparamType type;
    const EntryparamTraits traits;
} EntryparamTemplate;

static const EntryparamTemplate entryparamtemplate_list[] = {
    {"version", 'v', ENTRYPARAM_VERSION, ENTRYPARAM_VOID},
    {"help", 'h', ENTRYPARAM_HELP,ENTRYPARAM_VOID},
    {"input", 'i', ENTRYPARAM_INPUT, ENTRYPARAM_STRING | ENTRYPARAM_REQUIRED},
    {"output", 'o', ENTRYPARAM_OUTPUT,ENTRYPARAM_NUMBER | ENTRYPARAM_REQUIRED},
    {.type = ENTRYPARAM_NIL}
};

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
    auto type = entryparam_get_type(name);
    return type != ENTRYPARAM_NIL;
}

EntryparamType entryparam_get_type(const char* name) {
    u128 i = 0;
    const u128 il = (u128)strlen(name);
    for(; entryparamtemplate_list[i].type != ENTRYPARAM_NIL; ++i) {
        if(!strcmp(entryparamtemplate_list[i].long_name, name))
            return entryparamtemplate_list[i].type;
        if(il==1 && *name == entryparamtemplate_list[i].short_name)
            return entryparamtemplate_list[i].type;
    }
    return ENTRYPARAM_NIL;
}

void entryparam_destruct(Entryparam* self) {
    if(entryparam_get_traits(self->type) & ENTRYPARAM_STRING)
        free(self->payload.string);
}

EntryparamTraits entryparam_get_traits(const EntryparamType type) {
    u128 i = 0;
    for(; entryparamtemplate_list[i].type != ENTRYPARAM_NIL; ++i) {
        if(entryparamtemplate_list[i].type == type)
            return entryparamtemplate_list[i].traits;
    }

    return ENTRYPARAM_VOID;
}

typedef struct {char* adr; u8 offset; EntryparamType type; } _ArgvString;
typedef struct {double num; EntryparamType type; } _ArgvNumber;
typedef EntryparamType _ArgvVoid;

static char* _parse_argv_param(char* argv) {
    return argv + string_count_front(argv, '-');
}

EntryparamList entryparamlist_constrcut(int argc, char** argv) {
    EntryparamList epl = { .list = malloc((argc - 1)*sizeof(Entryparam)), .count = 0 };
    bool input_mode = false;
    u128 i;
    EntryparamTraits traits = ENTRYPARAM_VOID;

    for(i = 0; i < argc - 1; ++i) {
        // New parameter
        if(!input_mode && entryparam_isvalid_name(_parse_argv_param(argv[i + 1]))) {
            epl.list[epl.count].type = entryparam_get_type(_parse_argv_param(argv[i + 1]));
            traits = entryparam_get_traits(epl.list[epl.count].type);
            if(traits & ENTRYPARAM_NUMBER || traits & ENTRYPARAM_STRING) {
                input_mode = true;
                continue;
            }

            epl.count++;
        }

        if(input_mode) {
            // Read string/num and exit input mode
            if(traits & ENTRYPARAM_NUMBER) {
                epl.list[epl.count++].payload.number = atof(argv[i + 1]);
            } else
            if(traits & ENTRYPARAM_STRING) {
                epl.list[epl.count++].payload.string = argv[i + 1];
            }

            // TODO: Ending with " - leave long type mode and exit input mode
            // TODO: Starting with " - enter long type mode
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
        if(entryparam_get_traits(self->list[i].type) & ENTRYPARAM_NUMBER)
            printf("(%llu) %lf\n", self->count, self->list[i].payload.number);
        if(entryparam_get_traits(self->list[i].type) & ENTRYPARAM_STRING)
            printf("(%llu) %s\n", self->count, self->list[i].payload.string);
    }
}


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

