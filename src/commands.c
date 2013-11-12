#include "../include/commands.h"

void echo(int argc, char ** args){
	int i;

	if(argc==0){
		return;
	}

	printf("%s",args[0]);
	
	for(i=1;i<argc;i++){
		printf(" %s",args[i]);
	}
	
	printf("*\n");
}