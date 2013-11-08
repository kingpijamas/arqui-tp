#include "../include/kernel.h"

DESCR_INT idt[0x81];	// IDT de 81 entradas
IDTR idtr;				// IDTR

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
		//char ch=ZERO;
		//while(isEmpty()){
		//};

		char arg;
		//printf("%s","hola");
		scanf("%d",arg);

		//char* buffer;
		//__read(STD_IN, buffer,1);
		//__write(STD_OUT,buffer,1);

		//para probar write y getchar
		//ch= getChar();	
		//__write(STD_OUT,&ch,1);
	}
	
}

size_t __read(int fd, void* buffer, size_t count){
	size_t nread=_Sys_Call(SYS_READ,fd,buffer,count);
	return nread;
}

//WARNING, this function will assume that a character is being printed no matter what
//this is a problem when printing to REG_OUT
size_t __write(int fd, const void* buffer, size_t count){
	_Sys_Call(SYS_WRITE,fd,buffer,count);
	return count;
}