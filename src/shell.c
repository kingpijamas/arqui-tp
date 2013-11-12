#include "../include/shell.h"

char buffer[SHELL_BUFFER_SIZE]={NULL_CHAR};

ShellCommand commands[SHELL_COMMAND_COUNT]={
												{"echo",echo},
											};

void shell(){
	int promptLength;
	 promptLength=__draw_prompt();
	 __load_shell_buffer(promptLength);
	 __parse_shell_command();
	 __clear_shell_buffer();
}

void __load_shell_buffer(int promptLength){//FIXME weird behaviour here every now and then. Maybe it's bochs?
	char curr;
	int i=0,printed=promptLength;
	
	do{
		curr=readChar();

		if(curr=='\n'){
			printf("\n");
			return;
		}

		if(curr=='\b'){
			if(printed==promptLength){
				break;
			}else{
				//IMPORTANT: this is necessary because below 1 is being added below
				printed-=2;
			}
		}//FIXME: it takes \bs and puts them on the buffer: WRONG!

		buffer[i]=curr;
		printed+=iputc(buffer[i],STD_OUT);

		if(printed==LINE_WIDTH){
			printf("\n");
			printed=0;
		}

		i++;
	} while(i<SHELL_BUFFER_SIZE);
	// printf("salgo %d\n",i);
}

void __parse_shell_command(){
	ParsingStates state=NAME;
	char * name="\0";
	char * arg="\0";
	int i;

	for(i=0; buffer[i]!='\0'; i++){ //consider "" to escape whitespaces
		switch(buffer[i]){
			default:
				switch(state){
					case NAME:
						if(strcmp("\0",name)==0){
							name=buffer+i;
						}
						break;
					case ARG:
						if(strcmp("\0",arg)==0){
							arg=buffer+i;
						}
						break;
				}
				break;
			case ' ':
			case '\t':
				buffer[i]='\0';
				switch(state){
					case NAME:
						state=ARG;
						break;
					case ARG:
						return;//TODO: multiple args?
				}
				break;
		}
	}
	__invoke_shell_command(name,arg);	
}

void __invoke_shell_command(char * name, char * arg){
	int i;
	for(i=0;i<SHELL_COMMAND_COUNT;i++){
		if(strcmp(name,commands[i].name)==0){
			return (*(commands[i].cmd))(arg);
		}
	}
	return echo(name);//TODO helṕ
}

int __draw_prompt(){
	return printf("%s@%s:~%s",USER_NAME,PC_NAME,SHELL_PROMPT_END);
}

void __clear_shell_buffer(){
	int toClear;
	for(toClear=SHELL_BUFFER_SIZE;toClear>=0;toClear--){
		buffer[toClear]=NULL_CHAR;
	}
}