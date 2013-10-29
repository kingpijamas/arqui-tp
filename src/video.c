#include "../include/video.h"

int std_out_offset=0;
int reg_out_offset=0;

size_t __print(int fd, const void * buffer, size_t count){
	switch(fd){
		case STD_ERR:
		case STD_OUT:
			return __bounded_print(STD_OUT_MIN_ROW, STD_OUT_MIN_ROW, &std_out_offset, buffer, count);
		case REG_OUT:
			return __bounded_print(STD_OUT_MIN_ROW, STD_OUT_MIN_ROW, &reg_out_offset, buffer, count);
		default:
			return INVALID_DISPLAY;
	}
}

size_t __bounded_print(int fromRow, int toRow, int * offset, const void* buffer, size_t count){
	char *video = (char*)VIDEO_ADDRESS;
	int written;
	char c;

	for(written=0; written<count; written++){
		c=((char *)buffer)[written];
		switch(c){
			case '\n':
				(*offset)=__getOffsetOf(__getLineOf(*offset)+1);
				break;
			default:
				video[((*offset)++)*2]=c;
		}
	}
	return written;
}

int __getLineOf(int offset){
	return offset/WIDTH;
}

int __getOffsetOf(int line){
	return line*WIDTH;
}