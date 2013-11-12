#include "../include/shell.h"

static char shell_buffer[SHELL_BUFFER_SIZE]={NULL_CHAR};

static ShellCommand commands[SHELL_COMMAND_COUNT]={
												{"echo",echo},
											};

void shell(){
	int promptLength;
	 promptLength=__draw_prompt();
	 __load_shell_buffer(promptLength);
	 __parse_shell_command();
	 __clear_shell_buffer();
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

void __load_shell_buffer(int promptLength){//FIXME weird behaviour here every now and then. Maybe it's bochs?
	char curr;
	int i=0,printed=promptLength;

	do{
		// rprintf("aca llego\n");
		// __print_buffer();
		curr=readChar();
		// __print_buffer();
		// rprintf("\n");
		// __print_char_index(curr,i);
		// rprintf("\n");
		// __print_buffer();				

		// rprintf("A");
		if(curr=='\n'){
			// rprintf("salgo x aca\n");
			printf("\n");
			return;
		}
		// rprintf("B");
		if(curr=='\b'){
			// rprintf("B1");
			if(printed>promptLength+1){
				//IMPORTANT: this is necessary because below 1 is being added below
				//FIXME doesn't delete chars from the shell_buffer!
				printed-=2;
				// rprintf("borro:%d\n",i);
				shell_buffer[--i]='\0';
				// rprintf("\tbuffer[%d]=%c\n\n",i,curr);
				// __print_buffer();
				// rprintf("\n");
				// iputc('\b',STD_OUT);
			}
			break;
		}else{//FIXME: it takes \bs and puts them on the shell_buffer: WRONG!
			//FIXME: apparently there are \ns in the shell_buffer! <== cannot happen I think
			// rprintf("llega: ");
			// rprintf("C");
			// rprintf("\tbuffer[%d]=%c\n",i,curr);
			// __print_char(curr);
			// rprintf("\n");
			shell_buffer[i]=curr;
			printed+=iputc(shell_buffer[i],STD_OUT);

			if(printed==LINE_WIDTH){
				printf("\n");
				printed=0;
			}

			i++;
		}
		// rprintf("D");
		// rprintf("\ni=%d SHELL_BUFFER_SIZE=%d\n",i,SHELL_BUFFER_SIZE);
	} while(i<SHELL_BUFFER_SIZE);
	// rprintf("no, cagada\n");
	// rprintf("\ni=%d\n",i);
	printf("\n");
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
			case '\n':
				rprintf("problemas: \\n\n");//TODO just for debugging purposes
				break;
			case '\b':
				rprintf("problemas: \\b\n");//TODO just for debugging purposes
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
	echo(argc,args);
	return;
}

int __draw_prompt(){
	return printf("%s@%s:~%s",USER_NAME,PC_NAME,SHELL_PROMPT_END);
}

void __clear_shell_buffer(){
	int toClear;
	// rprintf("antes\n");
	// __print_buffer();
	for(toClear=SHELL_BUFFER_SIZE;toClear>=0;toClear--){
		shell_buffer[toClear]=NULL_CHAR;
	}
	// rprintf("\ndespues:");
	// __print_buffer();
	// rprintf("\n");
}