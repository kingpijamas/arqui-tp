#ifndef SYS_IO
#define SYS_IO

#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/video.h"
#include "../include/keyboard.h"

// System write
size_t __sys_write(int fd, const void* buffer, size_t count);

//System read
size_t __sys_read(int fd, void* buffer, size_t count);

size_t __readstdin(int fd, void* buffer, size_t count);

//Code taken from http://wiki.osdev.org/Inline_Assembly/Examples
static inline unsigned char inb( unsigned short port ) {
    unsigned char ret;
    asm volatile( "inb %1, %0"
                  : "=a"(ret) : "Nd"(port) );
    return ret;
}

//Code taken from http://wiki.osdev.org/Inline_Assembly/Examples
static inline void outb(unsigned short port, unsigned char val){
	asm volatile( "outb %0, %1"
		: : "a"(val), "Nd"(port) );
}

#endif