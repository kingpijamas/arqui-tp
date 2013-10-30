#ifndef STDIO_H
#define STDIO_H

#include "../include/kernel.h"
#include "../include/defs.h"
#include "../include/string.h"

typedef int FILE;

//check the
//	extern int __stdin=STD_IN
//	extern int* stdin = &__stdin
//
// + using FILE *
//
//	hack

//Auxiliary
typedef enum {WAITING, CHAR} format_t;

//TODO failure behavior not supported. Should we support it?
int fputc(int ch, FILE stream);
int putc(int ch, FILE stream);

//TODO check this. We're NOT using varargs
int fprintf(FILE stream, const char *format, const void ** args);
int printf(const char *format, const void ** args);

int __printUntil(FILE stream, const char * str, char limit);

//TODO just for debugging purposes
int auxPrint(const char * str);
#endif