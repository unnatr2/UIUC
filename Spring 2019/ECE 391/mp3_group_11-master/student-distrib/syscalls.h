#ifndef SYSCALL_ASM
#include "./types.h"

extern int32_t halt(uint8_t status);
extern int32_t execute(uint8_t* command);
extern int32_t read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t write(int32_t fd, const void* buf, int32_t nbytes);
extern int32_t open(uint8_t* filename);
extern int32_t close(int32_t fd);
extern int32_t getargs(uint8_t* buf, int32_t nbytes);

#endif
