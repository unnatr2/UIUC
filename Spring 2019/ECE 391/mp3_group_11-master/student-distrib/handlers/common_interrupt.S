#define CI_ASM 1
#include "common_interrupt.h"
#include "../macro_unrolling.h"
#define SYSCALL_VEC 0x80

.text

/* Globalize list of push-call function pointers for entry into idt */
#define GLOBALIZE_PCALL(num) \
    .globl pcall_interrupt_##num;
M_REPEAT_CALL(NUM_PCALLS, GLOBALIZE_PCALL)

/* Use repeated call macro to define all of the pcalls at preprocessor compile-time */
#define DECLARE_PCALL(num) \
    pcall_interrupt_##num: \
        pushl   $##num; \
        JMP common_interrupt;
M_REPEAT_CALL(NUM_PCALLS, DECLARE_PCALL)


/**
 * common_interrupt saves all registers, calls the corresponding interrupt
 * or exception handler and then restores all registers before calling IRET
 * to return control to interrupted program.
 * 
 * @param IDT Exception handler number (long) on the top of the stack
 */
common_interrupt:
    pushw %fs
    pushw $0
    pushw %es
    pushw $0
    pushw %ds 
    pushw $0
    pushl %eax
    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    /* 40 offset to handler number input pushed to stack */
    movl  40(%esp), %esi
    /* common_interrupt_jt defined in idt_init.c */
    call  *common_interrupt_jt(, %esi, 4)
    popl  %ebx
    popl  %ecx
    popl  %edx
    popl  %esi
    popl  %edi
    popl  %ebp
    /* If system call, preserve return %eax */
    cmpl  $SYSCALL_VEC, 16(%esp)
    je preserve_return
    popl  %eax
    jmp ci_continue
    preserve_return:
    addl $4, %esp
    ci_continue:
    /* jump over zero padding of ds */
    addl  $2, %esp
    popw  %ds  
    /* jump over zero padding of es */
    addl  $2, %esp
    popw  %es
    /* jump over zero padding of fs */
    addl  $2, %esp
    popw  %fs
    /* Pop handler number */
    addl  $4, %esp
    iret
