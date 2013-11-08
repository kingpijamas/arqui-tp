#include "../include/stdio.h"

int fputc(int ch, FILE stream){
	unsigned char auxChar=ch;
	if(__write(stream, &auxChar, 1)){
		return auxChar;
	}
	return EOF;
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
	int i,written=0;
	format_state_t state=PF_CHAR;
	for(i=0; format[i]!='\0'; i++){
		c=format[i];
		switch(state){
			case PF_CHAR:
				switch(c){
					case '%':
						state=PF_PARAMETER;
						break;
					default:
						written+=bfputc(c,stream);
				}
				break;
			case PF_PARAMETER:
				switch(c){
					// case '.':
					// 	state=PF_PRECISION;
					// 	break;
					case '%':
						written+=bfputc('%',stream);
						break;
					case 'c':
						//IMPORTANT: int here stands for 'char', va_arg won't work otherwise
						written+=bfputc(va_arg(vlist, int),stream);
						break;
					case 's':
						written+=__printString(stream,va_arg(vlist,char *));
						break;
					case 'd':
					case 'i'://TODO precision
						written+=__printDecimal(stream,va_arg(vlist,int));
						break;
					case 'o':
						written+=__printOctal(stream,va_arg(vlist,int));
						break;
					case 'x':
						written+=__printHexadecimal(stream,va_arg(vlist,int),false);
						break;
					case 'X':
						written+=__printHexadecimal(stream,va_arg(vlist,int),true);
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
						written+=__printDecimal(stream,written);
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
	}
	return written;
}

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

int __printInt(FILE stream, int i, int base, bool caps){
	int written=0;
	if(i<0){
		written+=bfputc('-',stream);
		i*=-1;
	}
	if(i>=base){
		written+=__printInt(stream,i/base,base,caps);
	}
	return written+__printDigit(stream,i%base,base,caps);
}

int __printDigit(FILE stream, int d, int base, bool caps){
	if(d<10){
		return bfputc('0'+d, stream);
	}
	if(d<16){
		if(caps){
			return bfputc('a'+d-10, stream);
		}else{
			return bfputc('A'+d-10, stream);
		}
	}
	//TODO will never happen
	return 0;
}

int scanf(const char *format, ...){
	int ret;
	va_list args;

	// Initialized args to retrieve the additonal arguments after format
	va_start(args,format);
	ret=vscanf(format, args);
	return ret;
}

bool isSpace(char c){
	return c==' ';
}

bool isTab(int c){
	return c=='\t';
}

bool isNumber(char c){
	return c>='0' && c<='9';
}

int scanfdecimal(int* arg, char curr){
	int i=0;
	while(!isSpace(curr) && isNumber(curr)){
		int number=(curr-'0');
		(*arg)=(*arg)*10+number;
		putc(curr,STD_OUT);
		curr=getChar();
	}
	return i==0?0:1;
}

/* Coloca chars del buffer en el parámetro arg, termina al recibir un enter.
Devuelve si colocó algo en args o no*/
int scanfstring(char* arg, char curr){
	int i=0;

	while(curr!='\n'){ 		
		if(curr!='\0'){ 
			arg[i++]=curr;
			putc(curr,STD_OUT);
		}
		curr=getChar();
	}
	arg[i]='\0';
	return i==0?0:1;
}

int scanfchar(char* arg, char curr){
	int i=0;
	while(curr!='\n'){
		if(curr!='\0'){
			arg[i++]=curr;
			putc(curr,STD_OUT);
		}
		curr=getChar();
	}
	return i;
}

int vscanf(const char * format, va_list args){

	char c,curr;
	int i=0,items=0;
	bool conversion;

	while(format[i]!='\0'){
		do {
			c=format[i];
		} while(isSpace(c) || isTab(c)); //Blanks or tabs are ignored

		curr=getChar();		

		//if(!isSpace(curr)){ //Ignora espacios
			if(c!='%'){
				if(c!=curr){
					return;
				}else{
					i++;
					putc(curr,STD_OUT);
				}
			}else{
				i++; 
				c=format[i];

				if (format[i]=='\0'){
					break;
				}

				switch(c){
					case 'd':
						{	int* arg;
							arg=va_arg(args,int*);
							if(isNumber(curr)){
								items=items+scanfdecimal(arg,curr);
							}
							i++;
							//putc(curr,STD_OUT);
							break;
						}
					case 's':
						{
							char* arg;
							arg=va_arg(args,char*);
							items=items+scanfstring(arg,curr);
							i++;
							printf("%s",arg);
							break;
						}
					case 'c':
						{
							char* arg;
							arg=va_arg(args,char*);
							items=items+scanfchar(arg,curr);
						}


				}
			}

		//}
		i++;
	}	
	return i;
}