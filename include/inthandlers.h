#ifndef INTHANDLERS_H
#define INTHANDLERS_H

#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/sysio.h"
#include "../include/kasm.h"

void int_08();
void int_09(char scancode, int gs);
size_t int_80(int sysCallNo, void** args);

#endif