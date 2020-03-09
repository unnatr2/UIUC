#define CONTEXT_SWITCH 1
#define USER_ESP 0x8400000 /* 132MB */

.text
.globl context_switch
.globl halt_ret

/**
 * push USER_DS, esp, eflags, cs, and eip. Call iret to go to user level
 * @param {uint32_t} user_ds -- pass in the USER_DS so we can access it in this .S file 
 * @param {uint32_t} user_cs -- pass in the USER_CS so we can access it in this .S file 
 * @param {uint32_t} entry_addrs -- address (for eip) of the next instruction for user program 
 * @param {uint32_t} pcb_bp_addrs -- pointer to pcb's exec_bp variable
 * @param {uint32_t} pcb_sp_addrs -- pointer to pcb's exec_sp variable
 * @return -- none
 * registers eax, ebx, edx are clobbered
 */
context_switch:
    /* Setup stack frame */
    pushl %ebp
    movl %esp, %ebp

    /* Put context_switch's stack info into pcb before IRET-ing*/
    movl 20(%ebp), %eax           /* Address of PCB's exec_ebpentry -> %eax */
    movl 24(%ebp), %ebx           /* Address of PCB's exec_espentry -> %ebx */
    movl %ebp, (%eax)             /* %ebp -> pcb->exec_ebp*/
    movl %esp, (%ebx)             /* %esp -> pcb->exec_esp= %esp */

    /* Get function arguments from stack */
    movl 8(%ebp), %eax            /* USER_DS */
    movl 12(%ebp), %ebx           /* USER_CS */
    movl 16(%ebp), %ecx           /* Entry Address */    
    /* Setup fake IRET Context */
    pushl %eax                    /* Push USER_DS */
    pushl $USER_ESP               /* Push User Stack Pointer */
    pushfl                        /* Push flags */
    pushl %ebx                    /* Push USER_CS */
    pushl %ecx                    /* Push Program Entry Address */
    iret                          /* Initiate context switch */
return_to_context_switch:
    popl %ebp
    ret
    

/**
 * restore esp and ebp
 * @param {uint32_t} exec_esp-- parent kernel stack's esp
 * @param {uint32_t} exec_ebp-- parent kernel stack's ebp
 * @param {uint8_t} status -- status of program that execute will return
 * @return -- status
 * clobbers eax and ecx registers
 */
halt_ret:
    movl 4(%esp), %edx    /* exec_esp*/
    movl 8(%esp), %ecx    /* exec_ebp*/
    movzwl 12(%esp), %eax /* Status of halted program */
    movl %edx, %esp
    movl %ecx, %ebp
    jmp return_to_context_switch
