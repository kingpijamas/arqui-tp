#ifndef SHELL_H
#define SHELL_H

#include "../include/stdio.h"

//TODO these are just for constants
#include "../include/keyboard.h"
#include "../include/video.h"

#define USER_NAME			"user"
#define PC_NAME				"PC"
#define SHELL_PROMPT_END	"$ "
#define NULL_CHAR			'\0'

#define SHELL_BUFFER_SIZE	WIDTH//SIZE_BUFFER //TODO: not the nicest name
#define LINE_WIDTH			WIDTH //WIDTH

void shell();
void __load_shell_buffer();
void __draw_shell();
void __echo(int promptLength);
int __draw_prompt();
void __clear_buffer(int toClear);

void __aux_fill_buffer(char* str);

#endif