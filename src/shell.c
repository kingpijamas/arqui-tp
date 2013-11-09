#include "../include/shell.h"

char buffer[SHELL_BUFFER_SIZE]={NULL_CHAR};

//FIXME
int testflag=0;

void shell(){
	__load_shell_buffer();
	__draw_shell();
}

void __aux_fill_buffer(char * str){
	int i;
	for(i=0;i<SHELL_BUFFER_SIZE && str[i]!='\0';i++){
		buffer[i]=str[i];
	}
}

void __load_shell_buffer(){
 	//scanf();
	int aux;
	if(testflag<5){
		aux=testflag%5;
		switch(aux){
			//case 0: __aux_fill_buffer("x");break;
			//case 1: __aux_fill_buffer("m\b\b\b");break;
			//case 2: __aux_fill_buffer("pepe es un mano\b\b\b\bmono");break;
			//case 3: __aux_fill_buffer("hola\n\b mundo!\n");break;
			case 4: __aux_fill_buffer("Baby, you've been going so crazy;Lately, nothing seems to be going right;Solo, why do you have to get so low;You're so...;You've been waiting in the sun too long");
			default:break;
		}
 	}
 	testflag++;
}

void __draw_shell(){
	int aux=__draw_prompt();
	__echo(aux);
}

int __draw_prompt(){
	return printf("%s@%s:~%s",USER_NAME,PC_NAME,SHELL_PROMPT_END);
}

//TODO \n\bs and \n\ts are not supported
void __echo(int promptLength){
	int i,printed=0;
	for (i=0;i<SHELL_BUFFER_SIZE && buffer[i]!=NULL_CHAR;i++) {
		if(buffer[i]=='\b'){
			if(printed==0){
				break;
			}else{
				//IMPORTANT: this is necessary because below 1 is being added below
				printed-=2;
			}
		}

		printed+=iputc(buffer[i],STD_OUT);

		if(printed==LINE_WIDTH){
			printf("\n");
			printed=0;
		}
	}
	__clear_buffer(i);
	printf("*");//FIXME here just for debugging purposes
	printf("\n");
}

void __clear_buffer(int toClear){
	for(;toClear>0;toClear--){
		buffer[toClear-1]=NULL_CHAR;
	}
}