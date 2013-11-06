#include "../include/sysio.h"


size_t __sys_write(int fd, const void* buffer, size_t count){
	switch(fd){
		case STD_ERR:
		case STD_OUT:
			__print(STD_DISPLAY,buffer,count);break;
		case REG_OUT:
			__print(REG_DISPLAY,buffer,count);break;
		default://TODO Should never happen. Maybe treat this as STD_OUT?
			return -1;
	}
}

size_t __sys_read(int fd, void* buffer, size_t count){
	size_t nread;

	switch(fd){
		case STD_IN: nread=__readstdin(fd, buffer,count);break;
		default: nread=-1; break;
	}
	return nread;
}

// TODO ver si moverlo a keyboard.c
size_t __readstdin(int fd, void* buffer, size_t count){
	size_t nread=0;
	char aux;

	while(count>0){
		aux=getChar();
		if(aux!=ZERO){
			((char *)buffer)[nread]=aux;
			count--;
			nread++;
		}
	}
	return nread;
}