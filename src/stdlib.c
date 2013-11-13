#include "../include/stdlib.h"

int dtoi(char digit){
	if(digit>='0' || digit<='9'){
		return digit-'0';
	}
	return -1;
}