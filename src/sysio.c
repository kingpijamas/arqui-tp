#include "../include/sysio.h"
#include "../include/keyboard.h"


size_t __sys_write(int fd, const void* buffer, size_t count){
	switch(fd){
		case STD_ERR:
		case STD_OUT:
		case REG_OUT:
			__print(fd,buffer,count);break;
		default:
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