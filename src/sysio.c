#include "../include/sysio.h"

int flag=0;

size_t __sys_write(int fd, const void* buffer, size_t count){
	switch(fd){
		case STD_ERR:
		case STD_OUT:
		case REG_OUT:
			if(flag%2==0){
				__print(STD_OUT,"buffer",6);
			}else{
				__print(REG_OUT,"pepe",4);
			}
			flag++;
		default:
			return 0;
	}
}