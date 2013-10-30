#include "../include/sysio.h"

size_t __sys_write(int fd, const void* buffer, size_t count){
	switch(fd){
		case STD_ERR:
		case STD_OUT:
		case REG_OUT:
			__print(fd,buffer,count);
		default:
			return -1;
	}
}