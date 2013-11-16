#include "../include/inthandlers.h"

void int_08() {
}

void int_09(unsigned char scancode, int eip, short int gs, short int fs, short int es, short int ds, short int ss, int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax){
	forBuffer(scancode,eip,gs,fs,es,ds,ss,edi,esi,ebp,esp,ebx,edx,ecx,eax);

}

size_t int_80(int sysCallNo, void ** args) {
	switch(sysCallNo){
		case SYS_WRITE:
			return __sys_write((int)args[0],(void*)args[1],(int)args[2]);
		case SYS_READ:
			return __sys_read((int)args[0],(void*)args[1],(int)args[2]);
		default:
			return -1;
	}
}
