#ifndef INTHANDLERS_H
#define INTHANDLERS_H

#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/sysio.h"
#include "../include/kasm.h"

void int_08();
void int_09(char scancode, int cs,int gs, int fs, int es, int ds, int ss, int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax, int eip);
size_t int_80(int sysCallNo, void** args);

#endif