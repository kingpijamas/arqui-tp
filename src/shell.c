#include "../include/shell.h"

static char shell_buffer[SHELL_BUFFER_SIZE]={'\0'};

static ShellCommand commands[SHELL_COMMAND_COUNT]={
												{"echo",echo_cmd},
												{"logo",logo_cmd},
												{"putc",putChar_test_cmd},
												{"printf",printf_test_cmd},
												{"getc",getChar_test_cmd},
												{"scanf",scanf_test_cmd},
												{"help",help_cmd}
												// {"clear",clear}
											};
static bool firstCall=true;

void shell(){
	if(firstCall){
		__print_welcome_message();
		firstCall=false;
		return;
	}

	int promptLength;
	 promptLength=__draw_prompt();
	 __load_shell_buffer(promptLength);
	 __parse_shell_command();
	 __clear_shell_buffer();
}

void __print_welcome_message(){
	logo_cmd(0,(char **)NULL);
	printf("%s\n",WELCOME_MSG);
}

void __print_char(char curr){
	switch(curr){
		case '\t':
			rprintf("\\t");
			break;
		case '\n':
			rprintf("\\n");
			break;
		case '\b':
			rprintf("\\b");
			break;
		default:
			rprintf("%c",curr);
			break;
	}
}

void __print_char_index(char curr, int i){
	switch(curr){
		case '\t':
			rprintf("\\t");
			break;
		case '\n':
			rprintf("\\n");
			break;
		case '\b':
			rprintf("\\b");
			break;
		default:
			rprintf("%c",curr);
			break;
	}
	rprintf("(%d) ",i);
}

void __print_buffer(){
	int i;
	rprintf("<BUF: ");
	for(i=0;i<15 || shell_buffer[i]!='\0';i++){
		__print_char_index(shell_buffer[i],i);
	}
	rprintf(" >");
}

void __load_shell_buffer(int promptLength){
	char curr;
	int i=0,printed=promptLength;

	do{
		curr=readChar();

		if(curr=='\n'){
			printf("\n");
			return;
		}

		if(curr=='\b'){
			if(printed>promptLength){
				printed--;
				shell_buffer[--i]='\0';
				iputc('\b',STD_OUT);
			}
		}else{
			shell_buffer[i]=curr;
			printed+=iputc(shell_buffer[i],STD_OUT);

			if(printed==LINE_WIDTH){
				printf("\n");
				printed=0;
			}

			i++;
		}
	} while(i<SHELL_BUFFER_SIZE);
}

void __parse_shell_command(){
	char * args[MAX_SHELL_ARGS+1]={"\0"};
	int i=0,argc=0;

	char c;
	bool expectingParam=true;
	while(shell_buffer[i]!='\0' && argc < MAX_SHELL_ARGS+1){
		c=shell_buffer[i];
		switch(c){
			default:
				if(expectingParam==true){
					args[argc]=shell_buffer+i;
					argc++;
					expectingParam=false;
				}
				break;
			case '\t':
			case ' ':
				shell_buffer[i]='\0';
				expectingParam=true;
				break;
		}
		i++;
	}
	__invoke_shell_command(argc,args);
}

void __invoke_shell_command(int argc, char ** args){
	int i;
	char * name=args[0];

	for(i=0;i<SHELL_COMMAND_COUNT;i++){
		if(strcmp(name,commands[i].name)==0){
			(*(commands[i].cmd))(argc-1,args+1);
			return;
		}
	}
	echo_cmd(argc,args);
	return;
}

int __draw_prompt(){
	return printf("%s@%s:~%s",USER_NAME,PC_NAME,SHELL_PROMPT_END);
}

void __clear_shell_buffer(){
	int toClear;
	for(toClear=SHELL_BUFFER_SIZE;toClear>=0;toClear--){
		shell_buffer[toClear]='\0';
	}
}