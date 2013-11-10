#include "../include/inthandlers.h"

#include "../include/stdio.h"
// #include "../include/video.h"

int _ppeax(void);

int flag=0;

void int_08() {
	if((flag++)){
		rprintf("%d\n",flag);
	}
	//printf("Baby, you've been going so crazy;Lately, nothing seems to be going right;Solo, \twhy do you have to get so low;You're so...;You've been waiting in the sun too long");
}

void int_09(char scancode, int gs){
	//_ppeax();
	forBuffer(scancode,gs);

}

size_t int_80(int sysCallNo, void ** args) {
	switch(sysCallNo){
		case SYS_WRITE:
			return __sys_write((int)args[0],(void*)args[1],(int)args[2]);
		case SYS_READ:
			return __sys_read((int)args[0],(void*)args[1],(int)args[2]);
		default:
			return -1;	//FIXME
	}
}
