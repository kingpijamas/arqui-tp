#include "../include/stdio.h"

int fputc(int ch, FILE stream){
	unsigned char auxChar=ch;
	__write(stream, &auxChar, 1);
	return auxChar; //TODO this assumes this function always succeeds
}

int putc(int ch, FILE stream){
	return fputc(ch,stream);
}

//TODO
int fprintf(FILE stream, const char *format, const void ** args){
	char c;
	int i=0, f=0;
	format_t state=CHAR;

	while(format[i]!='\0'){
		c=format[i];
		switch(state){
			case CHAR:
				switch(c){
					case '%':
						state=WAITING;
						break;
					default:
						//TODO could be done much more efficiently
						//TODO: this solution is bugged: i+=__printUntil(stream,format[i],'%');
						fputc(c,stream);
				}
			break;
			case WAITING:
				switch(c){
					case '%':
						fputc('%',stream);
					case 'c':
						fputc((char)args[f],stream);
						f++;
						break;
					case 's':
						//TODO validar que les guste esto y que no quieran que itere y use solo putc
						__write(stream,(char *)args[f],strlen((char *)args[f]));
						f++;
						break;
					case 'd':
					case 'i':
						//TODO do sth
						//TEMA: atoi((int)args[f]); hago una funcion especial que vaya putc-eando? el int
						f++;
						break;
					case 'o':
						//TODO do sth
						f++;
						break;
					case 'x':
						//TODO do sth
						f++;
						break;
					case 'X':
						//TODO do sth
						f++;
						break;
					case 'u':
						//TODO do sth
						f++;
						break;
					case 'f':
					case 'F':
						//TODO do sth
						f++;
						break;
					case 'e':
						//TODO do sth
						f++;
						break;
					case 'E':
						//TODO do sth
						f++;
						break;
					case 'a':
						//TODO do sth
						f++;
						break;
					case 'A':
						//TODO do sth
						f++;
						break;
					case 'g':
						//TODO do sth
						f++;
						break;
					case 'G':
						//TODO do sth
						f++;
						break;
					case 'n':
						//TODO do sth
						f++;
						break;
					case 'p':
						//TODO do sth
						f++;
						break;
				}
				state=CHAR;
			break;
		}
		i++;
	}
}

int printf(const char *format, const void ** args){
	return fprintf(STD_OUT,format,args);
}

int __printUntil(FILE stream, const char * str, char limit){
	int i;
	for(i=0;str[i]!=limit && str[i]!='\0';i++){;}
	__write(stream,str,i);
	return i;
}

int auxPrint(const char * str){
	__write(REG_OUT,str,strlen(str));
}