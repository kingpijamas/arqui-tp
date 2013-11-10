#include "../include/shell.h"

char buffer[SHELL_BUFFER_SIZE]={NULL_CHAR};

void shell(){
	int promptLength;
	promptLength=__draw_prompt();
	__load_shell_buffer(promptLength);
	__echo();
	__clear_shell_buffer();
}

void __load_shell_buffer(int promptLength){
	char curr;
	int i=0,printed=promptLength;
	
	do{
		curr=readChar();

		if(curr=='\n'){
			break;
		}

		if(curr=='\b'){
			if(printed==promptLength){
				break;
			}else{
				//IMPORTANT: this is necessary because below 1 is being added below
				printed-=2;
			}
		}

		buffer[i]=curr;
		printed+=iputc(buffer[i],STD_OUT);

		if(printed==LINE_WIDTH){
			printf("\n");
			printed=0;
		}

		i++;
	} while(i<SHELL_BUFFER_SIZE);
	
	printf("\n");
}

int __draw_prompt(){
	return printf("%s@%s:~%s",USER_NAME,PC_NAME,SHELL_PROMPT_END);
}

//TODO \n\bs and \n\ts are not supported
void __echo(){
	printf("%s*\n",buffer);
}

void __clear_shell_buffer(){
	int toClear;
	for(toClear=SHELL_BUFFER_SIZE;toClear>=0;toClear--){
		buffer[toClear]=NULL_CHAR;
	}
}