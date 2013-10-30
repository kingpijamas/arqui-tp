#include "../include/kernel.h"

DESCR_INT idt[0x81];	/* IDT de 81 entradas*/
IDTR idtr;				/* IDTR */

/**********************************************
kmain() 
Punto de entrada de codigo C.
*************************************************/
kmain() {
	int i,num;

	// Clears the screen
	k_clear_screen();

	// Carga de IDT con la rutina de atencion de IRQ0
	setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_hand, ACS_INT, 0);

	// Carga de IDTR
	idtr.base = 0;  
	idtr.base += (dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_Cli();

	// Habilito interrupcion de timer tick
	_mascaraPIC1(0x00); // Enable ALL
	_mascaraPIC2(0xFF); // Enable NONE
	_Sti();

	__init_graphics();

	while(1) {
	}
	
}

/* See kernel.h for description */
size_t __write(int fd, const void* buffer, size_t count){
	_Sys_Call(SYS_WRITE,fd,buffer,count);
	return count;
}
