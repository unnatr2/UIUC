#define SYSCALL_ENTRY 1
#define __ASM_FLAG__
#include "./syscall_handlers.h"

.text
.globl syscall_entry

/**
 * Entry point for system calls after jumping from IDT
 * Main job is to dispatch control to handlers based on %eax
 * Function does not setup a stack frame, nor should it! 
 *
 * @param %eax - syscall number as given in mp3.pdf
 */
syscall_entry:
    /* Validate syscall number here (1-10) */
    cmp $SYSCALLS_MAX, %eax
    jg syscall_bad
    cmp $SYSCALLS_MIN, %eax
    jl syscall_bad
    /* Jump to handler */
    decl %eax /* Zero-index */
    jmp *syscall_handler_jt(,%eax,4)
syscall_bad:
    movl $-1, %eax
    ret
