#ifndef VIDEO_H
#define VIDEO_H

#include "../include/defs.h"
//TODO just for debugging purposes
#include "../include/inthandlers.h"

#define VIDEO_ADDRESS	0xb8000
#define INVALID_DISPLAY -1

//TODO should I change all these for static consts?
#define WIDTH			80
#define HEIGHT  		25

#define MAX_COL			WIDTH-1
#define MAX_ROW 		HEIGHT-1

#define REG_OUT_HEIGHT	10
#define REG_OUT_MIN_ROW 0
#define REG_OUT_MAX_ROW REG_OUT_MIN_ROW+REG_OUT_HEIGHT-1

#define STD_OUT_HEIGHT	15
#define STD_OUT_MIN_ROW REG_OUT_MAX_ROW+1
#define STD_OUT_MAX_ROW STD_OUT_MIN_ROW+STD_OUT_HEIGHT-1

#define TAB_LENGTH		4

int __init_graphics();
size_t __print(int fd, const void * buffer, size_t count);
size_t __bounded_print(int minRow, int maxRow, int * offset, const void* buffer, size_t count);
int __getLineOf(int offset);
int __getOffsetOf(int line);

#endif