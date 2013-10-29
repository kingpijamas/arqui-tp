#ifndef VIDEO_H
#define VIDEO_H

#include "../include/defs.h"

#define VIDEO_ADDRESS	0xb8000
#define INVALID_DISPLAY -1

#define WIDTH			80

#define REG_OUT_HEIGHT	10
#define REG_OUT_MIN_ROW 0
#define REG_OUT_MAX_ROW REG_OUT_MIN_ROW+REG_OUT_HEIGHT-1

#define STD_OUT_HEIGHT	15
#define STD_OUT_MIN_ROW REG_OUT_MAX_ROW+1
#define STD_OUT_MAX_ROW STD_OUT_MIN_ROW+STD_OUT_HEIGHT-1

size_t __print(int fd, const void * buffer, size_t count);
size_t __bounded_print(int fromRow, int toRow, int * offset, const void* buffer, size_t count);
int __getLineOf(int offset);
int __getOffsetOf(int line);

#endif