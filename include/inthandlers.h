#ifndef INTHANDLERS_H
#define INTHANDLERS_H

#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/sysio.h"
#include "../include/kasm.h"

void int_08();
void int_09(unsigned char scancode, short unsigned int gs, short unsigned int fs, short unsigned int es, short unsigned int ds, short unsigned int ss, int edi, int esi, int ebx, int edx, int ecx, int esp, short unsigned int cs, int eip,int eax,unsigned short int flags, int ebp);
size_t int_80(int sysCallNo, void** args);

#endif