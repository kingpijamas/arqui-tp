#ifndef STDIO_H
#define STDIO_H

#include "../include/kernel.h"
#include "../include/defs.h"
#include "../include/string.h"
#include "../include/stdarg.h"

#define FILE FILE_DESC

//Auxiliary
typedef enum {PF_CHAR, PF_PARAMETER, PF_FLAGS, PF_WIDTH, PF_PRECISION, PF_LENGTH} format_state_t;

int fputc(int ch, FILE stream);
int putc(int ch, FILE stream);
#define bfputc(ch,stream)	((fputc(ch,stream)==ch)? 1:0)

int vfprintf(FILE stream, const char *format, va_list vlist);
int printf(const char *format, ...);
int fprintf(FILE stream, const char *format, ...);
int rprintf(const char *format, ...);

int __printUntil(FILE stream, const char * str, char limit);
int __printString(FILE stream, const char * str);
int __printInt(FILE stream, int i, int base, bool caps);
int __printDigit(FILE stream, int d, int base, bool caps);
#define __printOctal(stream,i)				__printInt(stream,i,8,false)
#define __printDecimal(stream,i)			__printInt(stream,i,10,false)
#define __printHexadecimal(stream,i,caps)	__printInt(stream,i,16,caps)

//TODO just for debugging purposes
int auxPrint(const char * str);

#endif