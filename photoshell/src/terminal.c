#include <application.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"

static ArgumentTemplate template[] = {
    {"version", 'v', ARGUMENT_VERSION, ARGUMENT_TRAIT_VOID | ARGUMENT_TRAIT_UNAMBIGUOUS},
    {"help", 'h', ARGUMENT_HELP,ARGUMENT_TRAIT_VOID | ARGUMENT_TRAIT_UNAMBIGUOUS},
    {"input", 'i', ARGUMENT_INPUT, ARGUMENT_TRAIT_STRING | ARGUMENT_TRAIT_REQUIRED | ARGUMENT_TRAIT_UNIQUE },
    {"output", 'o', ARGUMENT_OUTPUT,ARGUMENT_TRAIT_STRING | ARGUMENT_TRAIT_REQUIRED | ARGUMENT_TRAIT_UNIQUE },
    {.type = ARGUMENT_EOL}
};

#define TCOMMAND_NAME_MAX_LENGTH 8

typedef void(*TCommandFn)(void* application, i64 argc, i64* argv);

typedef struct {
    char name[TCOMMAND_NAME_MAX_LENGTH];
    TCommandFn fn;
    u8 argc;
} TCommand;

void tresize(Application* app, i64 argc, i64* argv) {
    application_perform_command(app, COMMAND_RESIZE, &(CommandResize){
        .w = argv[0],
        .h = argv[1]});
}

void tcrop(Application* app, i64 argc, i64* argv) {
    application_perform_command(app, COMMAND_CROP,  &(CommandCrop){
        .x = argv[0],
        .y = argv[1],
        .w = argv[2],
        .h = argv[3]});
}

void tcopy(Application* app, i64 argc, i64* argv) {
    application_perform_command(app, COMMAND_COPY,  &(CommandCopy){
        .x1 = argv[0],.x2 = argv[4],
        .y1 = argv[1],.y2 = argv[5],
        .w = argv[2],
        .h= argv[3]});
}

void tmove(Application* app, i64 argc, i64* argv) {
    application_perform_command(app, COMMAND_MOVE,  &(CommandMove){
        .x = argv[0],
        .y = argv[1]});
}

void tbw(Application* app, i64 argc, i64* argv)     { application_perform_command(app, COMMAND_BW, 0); }
void tinfo(Application* app, i64 argc, i64* argv)   { application_perform_command(app, COMMAND_INFO, 0);}
void tsave(Application* app, i64 argc, i64* argv)   { application_perform_command(app, COMMAND_SAVE, 0);}
void texit(Application* app, i64 argc, i64* argv)   { application_perform_command(app, COMMAND_EXIT, 0);}
void thelp(Application* app, i64 argc, i64* argv) {
    printf("\tresize w,h - resizes image to 'w' width and 'h' height.\n");
    printf("\tcrop x,y,w,h - crop image at ['x','y'] with 'w' width and 'h' height.\n");
    printf("\tcopy x1,y1,w,h x2,y2 - place copy from ['x1', 'y1'] with 'w' width and 'h' height to ['x2', 'y2']\n");
    printf("\tmove x,y - roll image 'x' to the left, 'y' down\n");
    printf("\tbw - converts to black and white\n");
    printf("\tinfo - prints info about the image\n");
    printf("\tsave - saves image to OUTPUT\n");
    printf("\texit - saves and exits program\n");
}

static TCommand commands[] = {
    {"resize",  (TCommandFn)tresize, 2 },
    {"crop",  (TCommandFn)tcrop, 4 },
    {"copy", (TCommandFn)tcopy, 6 },
    {"move", (TCommandFn)tmove, 2},
    {"bw", (TCommandFn)tbw,  0 },
    {"info",(TCommandFn)tinfo, 0 },
    {"save", (TCommandFn)tsave, 0 },
    {"exit", (TCommandFn)texit, 0 },
    {"help", (TCommandFn)thelp, 0 },
    {"eol", 0}
};


TCommand tcommand_find_by_name(const char* name) {
    u128 i = 0;
    while(strcmp("eol", commands[i].name)) {
        if(!strcmp(name, commands[i].name)) return commands[i];
        i++;
    }
    return commands[i];
}

void handle_line(Application* app, char* line) {
    const char* delim = " ,";
    char* token = strtok(line, delim);
    i64* data = 0, data_count = 0;
    TCommand command = tcommand_find_by_name(token);
    if(!strcmp(command.name, "eol")) {
        error_throw(ERROR_RUNT, "Unknown command, run 'help'.", false);
        return;
    }
    data = malloc(command.argc*sizeof(i64));
    do {
        token = strtok(NULL, delim);
        if(token && data_count < command.argc) {
            const i64 res = atoi(token);
            data[data_count++] = res;
        }
    } while(token);

    if(data_count == command.argc)
        command.fn(app, data_count, data);

    free(data);
}

int main(int argc, char** argv) {
    char buff[256] = { 0 };

    argument_set_template(template);

    Application app = application_construct(argc, argv);

    while(app.running) {
        fgets(buff, 256, stdin);
        //remove '\n'
        buff[strlen(buff) - 1] = 0;
        handle_line(&app, buff);
        errorhandler_handle();
    }

    application_destruct(&app);

    return 0;
}