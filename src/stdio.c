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

int rprintf(const char *format, ...){
	int ret;
	va_list args;

	va_start(args,format);
	ret=vfprintf(REG_OUT,format,args);
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
						//i+=__printUntil(stream,format[i],'%');
						fputc(c,stream);
				}
				break;
			case PF_PARAMETER:
				switch(c){
					// case '.':
					// 	state=PF_PRECISION;
					// 	break;
					case '%':
						fputc('%',stream);
						break;
					case 'c':
						//IMPORTANT: int here stands for 'char', va_arg won't work otherwise
						fputc(va_arg(vlist, int),stream);
						break;
					case 's':
						__printString(stream,va_arg(vlist,char *));
						break;
					case 'd':
					case 'i'://TODO precision
						__printInt(stream,va_arg(vlist,int));
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
	return __write(stream,str,i);
}

int auxPrint(const char * str){
	return __printString(REG_OUT,str);
}

int __printString(FILE stream, const char * str){
	return __write(stream,str,strlen(str));
}

int __printInt(FILE stream, int i){
	if(i<0){
		fputc('-',stream);
		i*=-1;
	}
	if(i>10){
		__printInt(stream,i/10);
	}
	__printDigit(stream,i%10);
}

int __printDigit(FILE stream, int d){
	if(d<10){
		return fputc('0'+d, stream);
	}
	//TODO will never happen
}