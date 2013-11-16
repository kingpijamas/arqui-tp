#include "../include/inthandlers.h"

void int_08() {
}

void int_09(unsigned char scancode, short unsigned int gs, short unsigned int fs, short unsigned int es, short unsigned int ds, short unsigned int ss, int edi, int esi, int ebx, int edx, int ecx, int esp, short unsigned int cs, int eip,int eax, int flags, int ebp){
	forBuffer(scancode,gs,fs,es,ds,ss,edi,esi,ebx,edx,ecx,esp,cs,eip,eax,flags,ebp);

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
