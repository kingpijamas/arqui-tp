
#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/keyboard.h"


static unsigned char keyboard_buffer[SIZE_BUFFER];
static int last=0; 
static int first=0; 
static bool full=false;
static int numletter=0;
static unsigned char lastscancode=0;

static bool lockFlag[LOCKSKEYS]={false,false,false}; //Num, Scrll, Caps
static bool specialKey[SPECIALSKEYS]={false,false,false}; //Control, Alt, Shift.


static unsigned char keyboard[KEYMAPROWS][KEYMAPSCOLS]={
{ZERO,ZERO,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t'}, //01 esc
{'q','w','e','r','t','y','u','i','o','p','[',']','\n',ZERO,'a','s'}, //L control
{'d','f','g','h','j','k','l',';','\'','`',ZERO,'\\','z','x','c','v'}, // L shift
{'b','n','m',',','.','/',ZERO,'*',ZERO,' ',ZERO,ZERO,ZERO,ZERO,ZERO,ZERO}, // R shift, L alt, Caps, f1, f2, f3, f4, f5
{ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,'-',ZERO,'5',ZERO,'+',ZERO}, //f6-f10, Num Lock
}; 

static unsigned char spKeyKeyboard[KEYMAPROWS][KEYMAPSCOLS] = {	
{ZERO,ZERO,'!','\"','#','$','%','^','&','*','(',')','_','+','\b','\t'},
{'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',ZERO,'A','S'},
{'D','F','G','H','J','K','L',';','[',']',ZERO,'>','Z','X','C','V'},	
{'B','N','M','<','>','?',ZERO,'*',ZERO,' ',ZERO,ZERO,ZERO,ZERO,ZERO,ZERO},
{ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,'7','8','9','-','4','6','+','1'},
{'2','3','0','.',ZERO,ZERO,ZERO,ZERO,ZERO}
};

bool isEmpty(){
	return (first==last && full==false);
}

/* Returns first element of the keyboard_buffer*/
char getKBChar(){
	if(isEmpty()){
		return '\0';
	}

	char caracter=keyboard_buffer[first];
	first++;

	if(first>=SIZE_BUFFER){
		first=bufferstart;
	}
	if(full){
		full=false;
	}
	return caracter;

}

void clearBuffer(){
	first=bufferstart;
	last=bufferstart;
	full=false;
}

bool isBreakCode(unsigned char scancode){
	return (scancode & 0x80);
}

bool isLetter(unsigned char ascii){
	return (ascii>=(unsigned char)'a' && ascii<=(unsigned char)'z')||(ascii>=(unsigned char)'A' && ascii<=(unsigned char)'Z');
}

bool isAscii(unsigned char ascii){
	return ascii!=ZERO;
}
void LockOnOff(int i){
	if(lockFlag[i]){
		lockFlag[i]=false;
	}else{
		lockFlag[i]=true;
	}	
}

void SpecialKeyOnOff(int i, bool onoff){
	specialKey[i-LOCKSKEYS]=onoff;
}

/* Returns special key's number if scancode is a specials key, else -1*/
int isSpecialKey(unsigned char scancode){
	int numKey=-1;
	switch(scancode){
		case NumLockScan: numKey=NumLock; break;
		case ScrllLockScan:numKey=ScrllLock; break;
		case CapsLockScan:numKey=CapsLock; break;
		case CtrlScan: numKey=Ctrl; break;
		case AltScan: numKey=Alt; break;
		case RShiftScan: numKey=Shift; break;
		case LShiftScan: numKey=Shift; break;
		default: numKey=-1; break;
//		case InsScanCode: break;
//		case DelScanCode: break;
	}
	return numKey;
}

void putinbuffer(unsigned char ascii){
	if(full==false){
		keyboard_buffer[last]=ascii;
		last++;
		if(last==SIZE_BUFFER && first!=bufferstart){
			last=bufferstart;
		}else if(last==first){
			full=true;
		}
	}				
}

void forBuffer(unsigned char scancode, short unsigned int gs, short unsigned int fs, short unsigned int es, short unsigned int ds, short unsigned int ss, int edi, int esi, int ebx, int edx, int ecx, int esp, short unsigned int cs, int eip, unsigned short int flags,int eax,int ebp){
    unsigned char ascii=ZERO;
   	int specialkeynum=-1; 
   	int specialindex=0;

	if(isBreakCode(scancode)){
		
		int makecode=scancode&0x7F;
		specialkeynum=isSpecialKey(makecode);
		specialindex=specialkeynum-LOCKSKEYS;

		if(specialkeynum>=LOCKSKEYS && specialKey[specialindex]){
			//If it's a breakcode we mind only for alt, control and shift.
			SpecialKeyOnOff(specialkeynum,false);
		}
		
		ascii=keyboard[makecode/KEYMAPSCOLS][makecode%KEYMAPSCOLS];
		if(makecode==lastscancode){
			lastscancode=0;
		}
	
		numletter--;
	
		return;
	}else{		
		ascii=keyboard[scancode/KEYMAPSCOLS][scancode%KEYMAPSCOLS];
		specialkeynum=isSpecialKey(scancode);	

		if(lastscancode!=scancode){
			numletter++;
		}
		
		if(isAscii(ascii)){
			lastscancode=scancode;
			if(isLetter(ascii)){
				if(lockFlag[CapsLock]){ 
					if(isLetter(ascii)){
						ascii=spKeyKeyboard[scancode/KEYMAPSCOLS][scancode%KEYMAPSCOLS];
					}else{
						ascii=keyboard[scancode/KEYMAPSCOLS][scancode%KEYMAPSCOLS];
					}
				}
				else if(specialKey[Ctrl-LOCKSKEYS] && (ascii=='r'||ascii=='R') && numletter==2 ){					
					
					if(numletter==2){ //Check only Control and r are pressed
						// CONTROL+R
						rprintf("eax:%xh\t\t",eax);			
						rprintf("ebx:%xh\t\t",ebx);
						rprintf("ecx:%xh\n",ecx);
						rprintf("edx:%xh\t\t",edx);
						rprintf("cs:%xh\t\t",cs);
						rprintf("gs:%xh\n",gs);
						rprintf("fs:%xh\t\t",fs);
						rprintf("es:%xh\t\t",es);
						rprintf("ds:%xh\n",ds);
						rprintf("ss:%xh\t\t",ss);
						rprintf("edi:%xh\t\t",edi);
						rprintf("esi:%xh\n",esi);
						rprintf("ebp:%xh\t\t",ebp);
						rprintf("esp:%xh\t\t",esp);
						rprintf("eip:%xh\n",eip);
						rprintf("flags:%xh",flags);
						rprintf("\n\n\n\n\n");
					}
					return;
				}else if(specialKey[Ctrl-LOCKSKEYS]){
					return;
				}			
			}
			if(specialKey[Shift-LOCKSKEYS]){
				ascii=spKeyKeyboard[scancode/KEYMAPSCOLS][scancode%KEYMAPSCOLS];
				
				if(lockFlag[CapsLock]){
					if(isLetter(ascii)){
						ascii=keyboard[scancode/KEYMAPSCOLS][scancode%KEYMAPSCOLS];
					}
				}
			}
			putinbuffer(ascii);
		}else if(specialkeynum>=0){
			if(lastscancode!=scancode){
				if(specialkeynum<LOCKSKEYS){	
					LockOnOff(specialkeynum);
				}else if(specialkeynum>=LOCKSKEYS){
					SpecialKeyOnOff(specialkeynum,true);
				}
				lastscancode=scancode;
				return;
			}
		}
	} 
}
