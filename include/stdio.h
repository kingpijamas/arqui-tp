#ifndef STDIO_H
#define STDIO_H

#include "../include/kernel.h"
#include "../include/defs.h"
#include "../include/string.h"
#include "../include/stdarg.h"

//check the
//	extern int __stdin=STD_IN
//	extern int* stdin = &__stdin
//
// + using FILE *
//
//	hack
typedef int FILE;

//Auxiliary
typedef enum {PF_CHAR, PF_PARAMETER, PF_FLAGS, PF_WIDTH, PF_PRECISION, PF_LENGTH} format_state_t;

//TODO failure behavior not supported. Should we support it?
int fputc(int ch, FILE stream);
int putc(int ch, FILE stream);

int vfprintf(FILE stream, const char *format, va_list vlist);
int printf(const char *format, ...);
int fprintf(FILE stream, const char *format, ...);
int rprintf(const char *format, ...);

int __printUntil(FILE stream, const char * str, char limit);
int __printString(FILE stream, const char * str);
int __printInt(FILE stream, int i);
int __printDigit(FILE stream, int d);

//TODO just for debugging purposes
int auxPrint(const char * str);

#endif