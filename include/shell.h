#ifndef SHELL_H
#define SHELL_H

#include "../include/defs.h"
#include "../include/stdio.h"
#include "../include/commands.h"
#include "../include/string.h"

#define USER_NAME			"user"
#define PC_NAME				"PC"
#define SHELL_PROMPT_END	"$ "

#define WELCOME_MSG			"Welcome to changOS!"

#define SHELL_BUFFER_SIZE	400
#define LINE_WIDTH			80

#define SHELL_COMMAND_COUNT 10
#define MAX_SHELL_ARGS		10


typedef struct {
    char * name;
    void (*cmd)(int,char**);
} ShellCommand;

void shell();

void __print_welcome_message();
void __load_shell_buffer(int promptLength);
void __parse_shell_command();
void __invoke_shell_command(int argc, char ** args);
int __draw_prompt();
void __clear_shell_buffer();


#endif