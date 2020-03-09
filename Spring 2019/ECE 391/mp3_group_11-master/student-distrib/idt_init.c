#include "idt_init.h"
#include "handlers/common_interrupt.h"
#include "handlers/exception_handlers.h"
#include "handlers/interrupt_handlers.h"
#include "handlers/syscall_entry.h"
#include "macro_unrolling.h"
#include "types.h"
#include "x86_desc.h"

/* Setup entries in IDT */
uint32_t TRAP_GATE_TYPE[4] = {0, 1, 1, 1};
uint32_t INTERRUPT_GATE_TYPE[4] = {0, 1, 1, 0};
uint32_t TASK_GATE_TYPE[4] = {0, 1, 0, 1};

/* Helper macro that calls insert_idt_entry for the
   corresponding handler number in the table and assigns
   that handler its corresponding pcall_interrupt handler */
#define INSERT_IDT_ENTRY_DEFAULT(handlerNum) \
    insert_idt_entry(                        \
        handlerNum,                          \
        pcall_interrupt_##handlerNum,        \
        DPL_KERNEL,                          \
        INTERRUPT_GATE_TYPE);

/* Declared here since cannot be declared in common_interrupt.h */
void* common_interrupt_jt[NUM_VEC] = {
    /* Default all non-specific interrupts to general handler */
    [0 ... ZERO_INDEX(NUM_VEC)] = handler_general_interrupt,
    /* Override specific handlers as seen in the x86 ISA */
    [0x00] = handler_except_div_by_zero,
    [0x01] = handler_except_debug_exception,
    [0x02] = handler_except_nmi,
    [0x03] = handler_except_breakpoint_exception,
    [0x04] = handler_except_overflow_exception,
    [0x05] = handler_except_bound_range_exceeded_exception,
    [0x06] = handler_except_invalid_opcode_exception,
    [0x07] = handler_except_device_not_available_exception,
    [0x08] = handler_except_double_fault_exception,
    [0x09] = handler_except_coprocessor_segment_overrun,
    [0x0A] = handler_except_invalid_tss_exception,
    [0x0B] = handler_except_segment_not_present,
    [0x0C] = handler_except_stack_fault_exception,
    [0x0D] = handler_except_general_protection_exception,
    [0x0E] = handler_except_page_fault_exception,
    [0x0F] = handler_general_interrupt,
    [0x10] = handler_except_FPU_floating_point_exception,
    [0x11] = handler_except_alignment_check_exception,
    [0x12] = handler_except_machine_check_exception,
    [0x13] = handler_except_SIMD_floating_point_exception,
    /* Override IRQ handlers as seen by the ECE391 course notes */
    [0x20] = handler_pic_irq0,
    [0x21] = handler_pic_irq1,
    [0x22] = handler_pic_irq2,
    [0x23] = handler_pic_irq3,
    [0x24] = handler_pic_irq4,
    [0x25] = handler_pic_irq5,
    [0x26] = handler_pic_irq6,
    [0x27] = handler_pic_irq7,
    [0x28] = handler_pic_irq8,
    [0x29] = handler_pic_irq9,
    [0x2A] = handler_pic_irq10,
    [0x2B] = handler_pic_irq11,
    [0x2C] = handler_pic_irq12,
    [0x2D] = handler_pic_irq13,
    [0x2E] = handler_pic_irq14,
    [0x2F] = handler_pic_irq15,
    /* System Call Handler/Entry-point */
    [0x80] = syscall_entry,
};

/**
 * Inserts an interrupt descriptor into the IDT with the specified Gate Type Bits
 * and handler. 
 * 
 * @param {int} VEC_NUM - the index in the IDT
 * @param {uint32_t} DPL - Descriptor privilege level for interrupt
 * @param {uint32_t} GATE_TYPE - Type bits of descriptor (eg: Trap, Interrupt, Task)
 */
void insert_idt_entry(int VEC_NUM, void* HANDLER, uint32_t DPL, uint32_t GATE_TYPE[4]) {
    idt[VEC_NUM].dpl = DPL;
    idt[VEC_NUM].seg_selector = KERNEL_CS;
    idt[VEC_NUM].reserved4 = 0;
    idt[VEC_NUM].reserved3 = GATE_TYPE[3];
    idt[VEC_NUM].reserved2 = GATE_TYPE[2];
    idt[VEC_NUM].reserved1 = GATE_TYPE[1];
    /* Determine if gate is a Task Gate (if so set size to 0) */
    if (GATE_TYPE[2] == 0) {
        idt[VEC_NUM].size = 0;
    } else {
        idt[VEC_NUM].size = 1;
    }
    idt[VEC_NUM].reserved0 = GATE_TYPE[0];
    idt[VEC_NUM].present = 1;
    /* Index into the list of push_calls in common_interrupt.S */
    SET_IDT_ENTRY(idt[VEC_NUM], HANDLER);
}

/**
 * idt_init initializes the IDT table and sets the gate type and handler, etc
 * for each entry so that when an exception is called, the corresponding action
 * is taken by the x86 architecture.
 * 
 * @return {void}
 */
void idt_init(void) {
    /* Insert pcall_interrupt handlers for all 256 entries in
       the IDT. This initializes all of the handlers to require
       kernel-level DPL and to be Interrupt Gate type handlers */
    M_REPEAT_CALL(NUM_VEC, INSERT_IDT_ENTRY_DEFAULT)

    /* Setup software interrupts */
    insert_idt_entry(0x80, pcall_interrupt_128, DPL_USER, TRAP_GATE_TYPE);
}
