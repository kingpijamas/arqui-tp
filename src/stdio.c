#include "../include/stdio.h"

int fputc(int ch, FILE stream){
	unsigned char auxChar=ch;
	__write(stream, &auxChar, 1);
	return auxChar; //TODO this assumes this function always succeeds
}

int putc(int ch, FILE stream){
	return fputc(ch,stream);
}

int fprintf(FILE stream, const char *format, ...){
	int ret;
	va_list args;

	va_start(args,format);
	ret=vfprintf(stream,format,args);
	va_end(args);

	return ret;
}

int printf(const char *format, ...){
	int ret;
	va_list args;

	va_start(args,format);
	ret=vfprintf(STD_OUT,format,args);
	va_end(args);

	return ret;
}

//TODO
//	%[parameter][flags][width][.precision][length]
int vfprintf(FILE stream, const char *format, va_list vlist){
	char c;
	int i=0;
	format_state_t state=PF_CHAR;
	while(format[i]!='\0'){
		c=format[i];
		switch(state){
			case PF_CHAR:
				switch(c){
					case '%':
						state=PF_PARAMETER;
						break;
					default://TODO could be done much more efficiently
						//TODO: this solution is bugged: i+=__printUntil(stream,format[i],'%');
						fputc(c,stream);
				}
				break;
			case PF_PARAMETER:
				switch(c){
					case '%':
						fputc('%',stream);
						break;
					case 'c':
						fputc(va_arg(vlist, int),stream);//int here stands for 'char', but va_arg won't work otherwise
						break;
					case 's':
						//TODO validar que les guste esto y que no quieran que itere y use solo putc
						//TODO could be much more efficient
						__printString(stream,va_arg(vlist,char *));
						// __write(stream,va_arg(args,(char *)),strlen((char *)args[f]));
						break;
					case 'd':
					case 'i':
						//TODO do sth
						//TEMA: atoi((int)args[f]); hago una funcion especial que vaya putc-eando? el int
						break;
					case 'o':
						//TODO do sth
						break;
					case 'x':
						//TODO do sth
						break;
					case 'X':
						//TODO do sth
						break;
					case 'u':
						//TODO do sth
						break;
					case 'f':
					case 'F':
						//TODO do sth
						break;
					case 'e':
						//TODO do sth
						break;
					case 'E':
						//TODO do sth
						break;
					case 'a':
						//TODO do sth
						break;
					case 'A':
						//TODO do sth
						break;
					case 'g':
						//TODO do sth
						break;
					case 'G':
						//TODO do sth
						break;
					case 'n':
						//TODO do sth
						break;
					case 'p':
						//TODO do sth
						break;
				}
				state=PF_CHAR;
				break;
			// case PF_FLAGS:
			// 	switch(c){
			// 		case '+':
			// 			break;
			// 		case ' ':
			// 			break;
			// 		case '-':
			// 			break;
			// 		case '#':
			// 			break;
			// 		case '0':
			// 			break;
			// 	}
			// 	break;
			// case PF_WIDTH:
			// 	break;
			// case PF_PRECISION:
			// 	break;
			// case PF_LENGTH:
			// 	break;
		}
		i++;
	}
}

//TODO bugged apparently
int __printUntil(FILE stream, const char * str, char limit){
	int i;
	for(i=0;str[i]!=limit && str[i]!='\0';i++){;}
	__write(stream,str,i);
	return i;
}

int auxPrint(const char * str){
	__printString(REG_OUT,str);
}

int __printString(FILE stream, const char * str){
	__write(stream,str,strlen(str));
}