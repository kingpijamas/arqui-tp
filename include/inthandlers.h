#ifndef INTHANDLERS_H
#define INTHANDLERS_H

#include "defs.h"

void int_08();
void int_80(int sysCallNo, int fd, void *buf, size_t count);
#endif