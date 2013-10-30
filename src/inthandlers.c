#include "../include/inthandlers.h"

int flag=0;

void int_08() {
	void* params[1]={"hola mundo!"};
	void* params2[1]={'*'};
	void** params3=0;
	void* params4[4]={"\n...wait for it!\n","\n...wait for it!\n","...dary!","\n\nLegen...dary!\n"};

	if(flag==0){
		printf("%s",params);
		printf("%c",params2);
		printf("%%",params);
		printf("\n\nLegen...\n%s%s\n%s%s",params4);
		flag++;
	}
	// putc('1',STD_OUT);
	// putc('2',REG_OUT);

	//__write(STD_OUT,"1",1);
	//__write(STD_OUT,"buffer",6);
	//}else{
	//	putc('b',REG_OUT);
		//__write(REG_OUT,"\bpepe",5);
	//}
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

//TODO just for testing purposes
void dummy_handler(void** args) {
	char *video = (char *) 0xb8000;
	int eax=(char)args[0];
	int ebx=(char)args[1];
	int ecx=(char)args[2];
	int edx=(char)args[3];

	video[34]=ctoi(eax);
	video[36]=ctoi(ebx);
	video[38]=ctoi(ecx);
	video[40]=ctoi(edx);
	return;
}

//TODO just for testing purposes
int ctoi(int c){
   	int aux;
   	switch(c){
	case 0:
			aux='0';
	break;
		case 1:
			aux='1';
			break;
		case 2:
			aux='2';
			break;
		case 3:
			aux='3';
			break;
		case 4:
			aux='4';
			break;
		case 5:
			aux='5';
			break;
		case 6:
			aux='6';
			break;
		case 7:
			aux='7';
			break;
		case 8:
			aux='8';
			break;
		case 9:
			aux='9';
			break;
		default:
			if(c > 10){
				aux='>';
			}else if (c>100){
				aux='~';
			}else{
				aux='*';
			}
	}
	return aux;
}