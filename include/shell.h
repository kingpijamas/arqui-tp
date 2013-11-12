#ifndef SHELL_H
#define SHELL_H

#include "../include/defs.h"
#include "../include/stdio.h"
#include "../include/commands.h"
#include "../include/string.h"

//TODO these are just for constants
#include "../include/keyboard.h"
#include "../include/video.h"

#define USER_NAME			"user"
#define PC_NAME				"PC"
#define SHELL_PROMPT_END	"$ "
#define NULL_CHAR			'\0'

#define SHELL_BUFFER_SIZE	SIZE_BUFFER //TODO: not the nicest name
#define LINE_WIDTH			WIDTH //WIDTH

#define SHELL_COMMAND_COUNT 1

typedef struct {
    char * name;
    void (*cmd)(char*);
} ShellCommand;

typedef enum {
	NAME,ARG
} ParsingStates;

void shell();
void __load_shell_buffer(int promptLength);
void __parse_shell_command();
void __invoke_shell_command(char * name, char * arg);
int __draw_prompt();
void __clear_shell_buffer();


#endif