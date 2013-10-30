#include "../include/video.h"

int std_out_offset;
int reg_out_offset;

int __init_graphics(){
	std_out_offset=__getOffsetOf(STD_OUT_MIN_ROW);
	reg_out_offset=__getOffsetOf(REG_OUT_MIN_ROW);
}

size_t __print(int fd, const void * buffer, size_t count){
	switch(fd){
		case STD_ERR:
		case STD_OUT:
			return __bounded_print(STD_OUT_MAX_ROW, &std_out_offset, buffer, count);
		case REG_OUT:
			return __bounded_print(REG_OUT_MAX_ROW, &reg_out_offset, buffer, count);
		default:
			return INVALID_DISPLAY;
	}
}

size_t __bounded_print(int maxRow, int * offset, const void* buffer, size_t count){
	char *video = (char*)VIDEO_ADDRESS;
	char c;
	int written, line;

	for(written=0; written<count && __getLineOf(*offset) < maxRow; written++){
		c=((char *)buffer)[written];
		switch(c){
			//TODO '\b'
			case '\n':
				(*offset)=__getOffsetOf(__getLineOf(*offset)+1);
				break;
			default:
				if(((*offset)+1) % MAX_COL == 0){
					(*offset)=__getOffsetOf(__getLineOf((*offset)+1));
				}
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