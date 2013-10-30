#include "../include/inthandlers.h"

int flag=0;

void int_08() {
	//rprintf("%s","hola mundo!");
	if(flag==0){
		//__printInt(STD_OUT,-1234567891);
		// printf("%%");
		// printf("%c",'*');
		printf("%d",-123);
		printf("\n\nLegen...\n%s%s\n%s%s","\n\t...wait for it!\n","\n\t\t...wait for it!\n","...dary!","\n\nLegen...dary!\n");
		//__printUntil(STD_OUT,"\n\nLegen...\na%s%s\n%s%s",'%');
		flag++;
	}
}

int s=80*11;

//s sirve para ir probando, deberia ocuparse el driver de video
void int_09(char scancode){
	putinbuffer(scancode,s);
	if(isScanCode(scancode)==0){
		s=s+2;
	}
}

void int_80(int sysCallNo, void ** args) {
	switch(sysCallNo){
		case SYS_WRITE:
			__sys_write((int)args[0],(void*)args[1],(int)args[2]);
			break;
		case SYS_READ:
			//TODO
			/*__sys_read(fd,buf,count);*/
		break;
		default:;
			//TODO	
	}
}