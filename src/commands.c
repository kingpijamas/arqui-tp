#include "../include/commands.h"

void echo_cmd(int argc, char ** args){
	int i;

	if(argc==0){
		return;
	}

	printf("%s",args[0]);
	
	for(i=1;i<argc;i++){
		printf(" %s",args[i]);
	}
	
	printf("\n");
}

void logo_cmd(int argc, char ** args){
	int i;
	for(i=0; strcmp("\0",logo[i])!=0; i++){
		printf("%s\n",logo[i]);
	}
}

// void printf_cmd(int argc, char ** args){

// }