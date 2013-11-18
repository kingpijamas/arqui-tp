#include "../include/commands.h"
#include "../include/kasm.h"

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

void __print_help_line(const char * cmdName,const char * description){
	printf("\t%s\t-\t%s\n",cmdName,description);
}

void help_cmd(int argc, char ** args){
	printf("List of available commands:\n");
	__print_help_line("echo    ","Displays parameter(s) in STD_OUT");
	__print_help_line("putc    ","Test for the 'putc' function from <stdio.h>");
	__print_help_line("printf  ","Test for the 'printf' function from <stdio.h>");
	__print_help_line("getc    ","Test for the 'getc' function from <stdio.h>");
	__print_help_line("scanf   ","Test for the 'scanf' function from <stdio.h>");
	__print_help_line("regs    ","Test for the register printing (changes registers in a loop)");
	__print_help_line("abrirCD ","Opens the CD reader");
	__print_help_line("cerrarCD","Closes the CD reader");
	__print_help_line("infoCD  ","Displays the size of the current CD in the CD reader");
	__print_help_line("logo    ","Prints the logo");
	__print_help_line("help    ","Prints a list of all available commands");
}

void logo_cmd(int argc, char ** args){
	int i;
	for(i=0; strcmp("\0",logo[i])!=0; i++){
		printf("%s\n",logo[i]);
	}
}

void putChar_test_cmd(int argc, char ** args){
	printf("Testing putc..\n\n\t");
	char c='A';
	putc(c,STD_OUT);
	printf("\n\nYou typed: '%c'\n",c);
}

void printf_test_cmd(int argc, char ** args){
	printf("Testing printf..\n\n");
	int num=3327;
	printf("\tPrinting an int (decimal notation).. %d\n",num);
	printf("\tPrinting the same int (octal notation).. %o\n",num);
	printf("\tPrinting the same int (hex notation).. %x\n",num);
	printf("\tPrinting the same int (HEX notation).. %X\n",num);
	printf("\tPrinting a string.. %s\n","hello world!");
	printf("\tThis: *\t* is a \\t\n");
	printf("\tThis: *\b* is a \\b\n");
	printf("\tThis: *\n*\t\t  is a \\n\n");
}

void getChar_test_cmd(int argc, char ** args){
	printf("Testing getChar..\n\n");

	printf("Please type a character:\n\n");
	printf("You typed...'%c'\n\n",getChar());
}

void scanf_test_cmd(int argc, char ** args){
	printf("Testing scanf..\n\n");

	char c;
	printf("Please type a character:\n");
	scanf("%c",&c);
	printf("\nYou typed...'%c'\n\n",c);

	int num=0;
	printf("Please type a number:\n");
	scanf("%d",&num);
	printf("\nYou typed...'%d'\n\n",num);

	printf("Please type any string:\n");
	char arg[200]={'\0'};
	scanf("%s",arg);
	printf("\nYou typed... '%s'\n\n",arg);


	// char * ans;
	// printf("Please type some text:\n");
	// scanf("%s",ans);
	// printf("%s",ans);
}

void openCD_cmd(int argc, char ** args){
	_ejectCD();
}
void infoCD_cmd(int argc, char ** args){
	_infoCD();
}
void closeCD_cmd(int argc, char ** args){
	_closeCD();
}

void regs_test_cmd(int argc, char ** args){
	printf("Incrementing registers eax, ebx and edx... (ecx is the timer)\n");
	_registerschange();
}