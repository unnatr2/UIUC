#include "pit_handler.h"
#include "../PCB.h"
#include "../i8259.h"
#include "../lib.h"
#include "../scheduling.h"
#include "syscall_handlers.h"

/**
 * Initializes the PIT on IRQ0 with the specified frequency
 * interval rate using a pulse hardware rate
 * 
 * @param {uint32_t} frequency - Interrupt frequency rate
 * @return {void} none
 */
void init_pit(uint32_t frequency) {
    uint32_t divisor = PIT_OPFREQ / frequency;
    uint8_t l = (divisor & BYTEMASK);
    uint8_t h = ((divisor >> BYTESHIFT) & BYTEMASK);

    { /* Critical Section begin */
        cli();
        outb(PIT_CONTROL_WORD, PIT_COMMAND_PORT);
        outb(l, PIT_DATA_PORT);
        outb(h, PIT_DATA_PORT);
        enable_irq(IRQ_PIT);
        sti();
    } /* Critical Section end */
}

/**
 * Handle each new PIT interrupt at the initialized
 * frequency interval
 * 
 * @return {void} none
 */
void handle_pit(void) {
    uint32_t next_pid = get_next_pid();
    send_eoi(IRQ_PIT);

    /* Halt process if the halt_flag is set in PCB */
    if (running_pcb()->halt_flag) syscall_handler_halt(254);

    /* Switch to next task */
    if (next_pid != ERRPID) switch_tasks(next_pid);
}
