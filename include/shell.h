#ifndef SHELL_H
#define SHELL_H

#include "../include/stdio.h"

#define USER_NAME			"user"
#define PC_NAME				"PC"
#define SHELL_PROMPT_END	"$"

#define SHELL_BUFFER_SIZE	100 //TODO: check!

void draw_shell();
void _draw_prompt();

#endif