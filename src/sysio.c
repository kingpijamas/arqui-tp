#include "../include/sysio.h"

//TODO change this
int offset=0;

size_t __sys_write(int fd, const void* buffer, size_t count){
	char *video = (char *) 0xb8000;
	int numwrite;
	switch(fd){
		case STD_OUT:
		case STD_ERR:
			video[offset+4]='z';
			break;
		default:
			video[offset+4]='x';
			break;
	}
	return numwrite;
}