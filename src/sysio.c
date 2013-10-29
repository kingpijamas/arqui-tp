#include "../include/sysio.h"

size_t __sys_write(int fd, const void* buffer, size_t count){
	switch(fd){
		case STD_ERR:
		case STD_OUT:
		case REG_OUT:
			return __print(fd,"buffer\n",7);
		default:
			return 0;
	}
}