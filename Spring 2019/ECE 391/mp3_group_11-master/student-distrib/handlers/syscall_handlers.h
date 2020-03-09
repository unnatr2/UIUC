/* Make VSCode Intellisense happy :) */
#ifndef asmlinkage
#define asmlinkage
#endif

#define SYSCALLS_MAX 10
#define SYSCALLS_MIN 1
#define MIN_FD 2
#define MAX_FD 7

#ifndef __ASM_FLAG__
#undef __ASM_FLAG__
#ifndef SYSCALL_HANDLERS
#define SYSCALL_HANDLERS

#include "../PCB.h"
#include "../drivers/filesystem_driver.h"
#include "../handlers/keyboard_handler.h"
#include "../types.h"

extern void* syscall_handler_jt[SYSCALLS_MAX];

extern asmlinkage int32_t syscall_handler_halt(uint8_t status);
extern asmlinkage int32_t syscall_handler_execute(uint8_t* command);
extern asmlinkage int32_t syscall_handler_read(int32_t fd, void* buf, int32_t nbytes);
extern asmlinkage int32_t syscall_handler_write(int32_t fd, const void* buf, int32_t nbytes);
extern asmlinkage int32_t syscall_handler_open(uint8_t* filename);
extern asmlinkage int32_t syscall_handler_close(int32_t fd);
extern asmlinkage int32_t syscall_handler_getargs(uint8_t* buf, int32_t nbytes);
extern asmlinkage int32_t syscall_handler_vidmap(uint8_t** screen_start);
extern asmlinkage int32_t syscall_handler_set_handler(int32_t signum, void* handler_address);
extern asmlinkage int32_t syscall_handler_sigreturn(void);

void parseCommand(uint8_t* command, uint8_t executable_buffer[FILE_NAME_SIZE], uint8_t args_buffer[KEYBOARD_BUFFER_SIZE]);

#endif /* __ASM_FLAG__ */
#endif /* SYSCALL_HANDLERS */
