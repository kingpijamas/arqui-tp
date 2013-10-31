#include "../include/shell.h"

void draw_shell(){

}

void _draw_prompt(){
	printf("%s@%s:~%s\n",USER_NAME,PC_NAME,SHELL_PROMPT_END);
}