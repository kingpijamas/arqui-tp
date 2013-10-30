#ifndef INTHANDLERS_H
#define INTHANDLERS_H

#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/sysio.h"

void int_08();
void int_09(char scancode);
void int_80(int sysCallNo, void** args);

//TODO just for testing purposes
void dummy_handler(void** args);
//TODO just for testing purposes
int ctoi(int c);
#endif