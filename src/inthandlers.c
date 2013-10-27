#include "../include/inthandlers.h"

void int_08() {
/*
    char *video = (char *) 0xb8000;
    video[tickpos+=2]='*';
 */
}

void int_80(int sysCallNo, int fd, void *buf, size_t count) {
	switch(sysCallNo){
		case SYS_WRITE:
			__write(fd,buf,count);
			break;
		case SYS_READ:
			//TODO
			/*__read(fd,buf,count);*/
			break;
		default:;
			//TODO	
	}
}