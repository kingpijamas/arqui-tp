#include "../include/inthandlers.h"

int tickpos=0;

void int_08() {
    //test
    // char *video = (char *) 0xb8000;
    // video[tickpos+=2]='a';

	//TODO just for testing purposes
	__write(STD_OUT,NULL,0);
}

void int_80(int sysCallNo, void ** args) {
	char *video = (char *) 0xb8000;

	video[32]='=';
	video[10]=ctoi(sysCallNo);
	video[14]=ctoi(SYS_WRITE);

	switch(sysCallNo){
		case SYS_WRITE:
			switch((int)args[0]){
				case STD_OUT:
				case STD_ERR:
					video[2]='a';
					break;
				default:
					video[2]='s';
					break;
			}
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
			aux='*';
	}
	return aux;
}