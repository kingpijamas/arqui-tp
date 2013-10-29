#ifndef SYS_IO
#define SYS_IO

#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/video.h"

// System write
size_t __sys_write(int fd, const void* buffer, size_t count);
#endif