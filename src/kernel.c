#include "../include/kernel.h"
#include "../include/atapi.h"

static DESCR_INT idt[0x81];	// IDT de 81 entradas
static IDTR idtr;			// IDTR

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

	// ata_identify(0x1F0,ATA_DRIVE_MASTER);
	// ata_identify(ATA_BUS_SECONDARY,ATA_DRIVE_MASTER);

	// ata_identify(0x170,0xB0);
	// ata_identify(0x1F0,0xB0);

	// atapi_drive_start_stop();

	int CD_BUS;
	int CD_DRIVE;

	int ata;
	int j;
	while(true) {
			
		printf("hola");
		// ata_sreset(ATA_BUS_SECONDARY);
		// ata_sreset(ATA_BUS_PRIMARY);

		
		// rprintf("Slave secundario:");
		// ata=ata_identify(ATA_BUS_SECONDARY,ATA_DRIVE_SLAVE);
		// if(ata==ISCD){
		// 	CD_BUS=ATA_BUS_SECONDARY;
		// 	CD_DRIVE=ATA_DRIVE_SLAVE;
		// }
		// rprintf("%d\n", ata);

		// rprintf("Master Secundario:");
		// ata=ata_identify(ATA_BUS_SECONDARY,ATA_DRIVE_MASTER);
		// if(ata==ISCD){
		// 	CD_BUS=ATA_BUS_SECONDARY;
		// 	CD_DRIVE=ATA_DRIVE_SLAVE;
		// }
		// rprintf("%d\n", ata);

		// rprintf("Slave primario:");
		// ata=ata_identify(ATA_BUS_PRIMARY,ATA_DRIVE_SLAVE);
		// if(ata==ISCD){
		// 	CD_BUS=ATA_BUS_SECONDARY;
		// 	CD_DRIVE=ATA_DRIVE_SLAVE;
		// }
		// rprintf("%d\n", ata);

		// rprintf("Master Primario:");
		// ata=ata_identify(ATA_BUS_PRIMARY,ATA_DRIVE_MASTER);
		// if(ata==ISCD){
		// 	CD_BUS=ATA_BUS_SECONDARY;
		// 	CD_DRIVE=ATA_DRIVE_SLAVE;
		// }
		// rprintf("%d\n",ata);
		// printf("%x\n",CD_BUS);
		// printf("%x\n",CD_DRIVE);

		// ata_sreset(ATA_BUS_SECONDARY);
		// ata_sreset(ATA_BUS_PRIMARY);
		
		// atapi_drive_startstop(ATA_DRIVE_SLAVE,ATA_BUS_SECONDARY);
		//rprintf("closing");
		//_ejectCD();
		// int size=atapi_drive_startstop(ATA_DRIVE_SLAVE,ATA_BUS_SECONDARY);
		//printf("fin");
		// rprintf("%d",size);

		// char ch='\0';
		// char ch='\0';

		//TODO take a look at this
		//char* buffer;
		//__read(STD_IN, buffer,2);
		
		//SCANF WORKING
		 //char *arg;
		 //scanf("%s",arg);

		//GET CHAR AND WRITE WORKING!
		// while(ch=='\0'){
		// 	ch= getChar();	
		// }
		// //__write(STD_OUT,&ch,1);
		
		// //GET CHAR AND PUTCHAR WORKING!
		// //while(ch=='\0'){
		// //	ch= getChar();	
		// //}
		// putc(ch,STD_OUT);
		
		// shell();

		//bugean
		
	while(true) {		
		shell();
	}
	
}

size_t __read(int fd, void* buffer, size_t count){
	return _Sys_Call(SYS_READ,fd,buffer,count);
}

size_t __write(int fd, const void* buffer, size_t count){
	_Sys_Call(SYS_WRITE,fd,buffer,count);
	return count;
}
