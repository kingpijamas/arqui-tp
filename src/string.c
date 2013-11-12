#include "../include/string.h"

size_t strlen(const char *s){
	int i;
	for(i=0;s[i]!='\0';i++){;}
	return i;
}

int strcmp(char * s1, char * s2){
	int i;
	for(i=0; s1[i]!='\0' && s2[i]!='\0' && s1[i]==s2[i]; i++){
	}
	if(s1[i]=='\0'){
		if(s2[i]!='\0'){
			return 1;
		}
		return 0;
	}
	return -1;
}