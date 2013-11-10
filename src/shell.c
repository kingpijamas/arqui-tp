#include "../include/shell.h"

char buffer[SHELL_BUFFER_SIZE]={NULL_CHAR};

int test=0;

void shell(){
	// if(test==0){
	// 	printf("hola mono\b");
	// }
	// test++;
	int promptLength;
	promptLength=__draw_prompt();
	// rprintf("antes:%s*\n",buffer);
	__load_shell_buffer(promptLength);
	// rprintf("despues:%s*\n",buffer);
	__echo();
	__clear_shell_buffer();
}

void __load_shell_buffer(int promptLength){
	int i,printed;
	char curr;

	for(i=0,printed=promptLength; i<SHELL_BUFFER_SIZE; i++){
		curr=readChar();

		if(curr=='\n'){
			printf("\n");
			return;
		}
		
		// rprintf("\t%c",curr);

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
	}
	printf("\n");//TODO: could never get here unless SHELL_BUFFER_SIZE<KEYBOARD_BUFFER_SIZE
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