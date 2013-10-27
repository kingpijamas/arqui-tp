#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/inthandlers.h"


DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

int tickpos=640;

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

	setup_IDT_entry (&idt[0x80], 0x80, (dword)&_int_80_hand, ACS_INT, 0);

	// Carga de IDTR
	idtr.base = 0;  
	idtr.base += (dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_Cli();

	// Habilito interrupcion de timer tick
	_mascaraPIC1(0xFC); /* Timer tick y teclado */
	_mascaraPIC2(0xFF);

	_Sti();

	while(1) {
	}
	
}

/* See kernel.h for description */
size_t __write(int fd, const void* buffer, size_t count){
	int numwrite;

	switch(fd){
		/*CASE STANDAR_OUTPUT: numwrite videoprint(buffer,count,video);*/
	}

	return numwrite;
}
