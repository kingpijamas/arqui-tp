#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../include/defs.h"
#include "../include/kasm.h"

#define SIZE_BUFFER 20
#define KEYMAPROWS 8
#define KEYMAPSCOLS 16
#define bufferstart 0

#define ZERO 0

typedef int bool;
#define true 1
#define false 0

#define NumLockScan 0x45
#define ScrllLockScan 0x46
#define CapsLockScan 0x3a
#define CtrlScan 0x1d
#define AltScan 0x38
#define RShiftScan 0x36
#define LShiftScan 0x2a
#define InsScanCode 0x52


#define LOCKSKEYS 3
#define NumLock 0
#define ScrllLock 1
#define CapsLock 2

#define SPECIALSKEYS 3
#define FIRSTSPECIALKEY 3
#define Ctrl 3
#define Alt 4
#define Shift 5

#define Ins 6
#define Del 7

bool isEmpty();

char getChar();

void clearBuffer();

bool isBreakCode(unsigned char scancode);

bool isLetter(unsigned char ascii);

bool isAscii(unsigned char ascii);

void LockOnOff(int i);

void SpecialKeyOnOff(int i, bool onoff);

int isSpecialKey(unsigned char scancode);

void putinbuffer(unsigned char ascii);

void forBuffer(unsigned char scancode,int gs, int fs,int es, int ds, int ss, int edi,int esi, int ebp, int esp,int ebx, int edx, int ecx, int eax, int cs, int eip);

#endif