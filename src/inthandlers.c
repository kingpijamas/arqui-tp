#include "../include/inthandlers.h"

int flag=0;

void int_08() {
	//rprintf("%s","hola mundo!");
	// int i;
	// if (flag<10){
	// 	// for(i=0;i<50;i++){
	// 	// 	printf("%d",flag);
	// 	// }
	// 	// printf("\n");
	// }
	//_draw_prompt();

	if(flag==0){
		// rprintf("reg_out\tminRow=%d\tmaxRow=%d\t",REG_OUT_MIN_ROW,REG_OUT_MAX_ROW);
		// rprintf("\nstd_out\tminRow=%d\tmaxRow=%d\t",STD_OUT_MIN_ROW,STD_OUT_MAX_ROW);
		// rprintf("\nda=%d",REG_OUT_MAX_ROW+1);
		// printf("%%");
		// printf("%c",'*');
		printf("d:%d\n",15);
		printf("x:%x\n",15);
		printf("X:%X\n",15);
		printf("o:%o\n",15);

		//rprintf("\n\nLegen...\n%s%s\n%s%s","\n\t...wait for it!\n","\n\t\t...wait for it!\n","...dary!","\n\nLegen...dary!\n");
	}
	if(flag==10){
		//__shift_up(STD_OUT, 1);
	}
	flag++;
}

int s=80*11;

//s sirve para ir probando, deberia ocuparse el driver de video
void int_09(char scancode){
	putinbuffer(scancode,s);
	if(isScanCode(scancode)==0){
		s=s+2;
	}
}

void int_80(int sysCallNo, void ** args) {
	switch(sysCallNo){
		case SYS_WRITE:
			__sys_write((int)args[0],(void*)args[1],(int)args[2]);
			break;
		case SYS_READ:
			//TODO
			/*__sys_read(fd,buf,count);*/
		break;
		default:;
			//TODO	
	}
}