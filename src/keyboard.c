#include "../include/keyboard.h"

#define SIZE_BUFFER 20
#define SCTABLEROWS 8
#define SCTABLECOLS 16

#define start 0

/* vacio igual a lleno? Problema buffer circular*/

char buffer[SIZE_BUFFER];
int last=0; /*marca el proximo lugar disponible*/
int first=0; /*marca el primer elemento de mi buffer*/ 
bool full=false;

/* keyboard[scancode%16][scancode/16] */
unsigned char keyboard[SCTABLEROWS][SCTABLECOLS]={
{0,0,'1','2','3','4','5','6','7','8','9','0','\'','¿','\b','\t'},
{'q','w','e','r','t','y','u','i','o','p','\'','+','\n',0,'a','s'},
{'d','f','g','h','j','k','l','.','{','}',0,'<','z','x','c','v'},
{'b','n','m',',','.','?',0,'*',0,' ',0,0,0,0,0,0},
{0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1'},
{'2','3','0','.',0,'+','+',0,0,'+','+'}};

void putinbuffer(int scancode, unsigned int s) {	
   	char *video = (char *) 0xb8000;
	if(isScanCode(scancode)){

	}else{
		if(full==false){
			buffer[last]=keyboard[scancode/SCTABLECOLS][scancode%SCTABLECOLS];
			last++;

			if(last==first){
				full=true;
			}else if(last==SIZE_BUFFER && first!=start){
				last=start;
			}
		}
		
		//para probar
		video[s++]=keyboard[scancode/SCTABLECOLS][scancode%SCTABLECOLS];
		video[s]=0x03;	
	}

}

bool isScanCode(int scancode){
	return (scancode & 0x80);
}


