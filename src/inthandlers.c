#include "../include/inthandlers.h"

int flag=0;

int _ppeax(void);

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
	//__set_cursor_position(flag++,0);

	/*if (flag%2==0){
		__write(STD_OUT,"buffer",6);
	}else{
		__write(REG_OUT,"pepe",4);
	}
	flag++;*/

	//TODO just for testing purposes
	//__write(STD_OUT,NULL,0);

	// if(flag%2==0){
	// 	__set_cursor_position_in(STD_OUT,2,flag);		
	// }else{
	// 	__set_cursor_position_in(REG_OUT,2,flag);
	// }

	if(flag==0){
		// rprintf("reg_out\tminRow=%d\tmaxRow=%d\t",REG_OUT_MIN_ROW,REG_OUT_MAX_ROW);
		// rprintf("\nstd_out\tminRow=%d\tmaxRow=%d\t",STD_OUT_MIN_ROW,STD_OUT_MAX_ROW);
		// rprintf("\nda=%d",REG_OUT_MAX_ROW+1);
		// printf("%%");
		// printf("%c",'*');
		// printf("d:%d\n",12345);
		// printf("x:%x\n",12345);
		// printf("X:%X\n",12345);
		// printf("o:%o\n",12345);
		// update_cursor();

		// rprintf("largo:%d\n",strlen("\n\nLegen...\n\n")+
		// 	strlen("\n\t...wait for it!\n")+
		// 	strlen("\n\t\t...wait for it!\n")+
		// 	strlen("...dary!")+
		// 	strlen("\n\nLegen...dary!\n"));

		// rprintf("largo:%d\n",printf("\n\nLegen...\n%s%s\n%s%s","\n\t...wait for it!\n","\n\t\t...wait for it!\n","...dary!","\n\nLegen...dary!\n"));
		//	printf("\n\nLegen...\n%s%s\n%s%s","\n\t...wait for it!\n","\n\t\t...wait for it!\n","...dary!","\n\nLegen...dary!\n");
		// rprintf("da: %d\n",__printUntil(REG_OUT,"hola\n mundo!",' '));
		// rprintf("deberia dar: %d\n",strlen("hola\n"));
	}
	if(flag==10){
		//__shift_up(STD_OUT, 1);
	}
	flag++;
}

void int_09(char scancode, int gs, int fs,int es, int ds, int ss, int edi,int esi, int ebp, int esp,int ebx, int edx, int ecx, int eax, int cs,int eip){
	//_ppeax();
	forBuffer(scancode,gs,fs,es,ds,ss,edi,esi,ebp,esp,ebx,edx,ecx,eax,cs,eip);

}

size_t int_80(int sysCallNo, void ** args) {
	size_t nsize;
	switch(sysCallNo){
		case SYS_WRITE:
			__sys_write((int)args[0],(void*)args[1],(int)args[2]);
			break;
		case SYS_READ:
			nsize=__sys_read((int)args[0],(void*)args[1],(int)args[2]);
		break;
		default:;
			//TODO
	}
	//FIXME: shouldn't we return here?
}
