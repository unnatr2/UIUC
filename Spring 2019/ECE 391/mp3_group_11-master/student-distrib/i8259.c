/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask = MASK_ALL; /* IRQs 0-7  */
uint8_t slave_mask = MASK_ALL;  /* IRQs 8-15 */

/**
 * Initializes the i8259 PIC with the processor and instructs
 * the PIC to accept interrupts on IDT entries 0x20-0x2F
 * 
 * @param {void} no parameters
 * @return {void}
 */
void i8259_init(void) {
    cli();  // Clear all interrupts

    outb(master_mask, MASTER_DATA_PORT);  // Mask all interrupts for Master PIC
    outb(slave_mask, SLAVE_DATA_PORT);    // Mask all interrupts for Slave PIC

    outb(ICW1, MASTER_8259_PORT);         // write control word 1 to start initializing the Master PIC
    outb(ICW2_MASTER, MASTER_DATA_PORT);  // set up Master Pic's data I/O port (at MASTER_8259_PORT + 1)
    outb(ICW3_MASTER, MASTER_DATA_PORT);  // set up cascading on Master (where the slave will be on the master)
    outb(ICW4, MASTER_DATA_PORT);         // write CW4 for end of Master PIC initialization

    outb(ICW1, SLAVE_8259_PORT);        // write control word 1 to start initializing the Slave PIC
    outb(ICW2_SLAVE, SLAVE_DATA_PORT);  // set up Slave Pic's data I/O port (at SLAVE_8259_PORT + 1)
    outb(ICW3_SLAVE, SLAVE_DATA_PORT);  // set up cascading on Slave (where the slave will be on the master)
    outb(ICW4, SLAVE_DATA_PORT);        // write CW4 for end of Slave PIC initialization

    (void)enable_irq(IRQ_SLAVE);  // Enable Slave IRQ on Master PIC

    sti();  // Set all interrupts
}

/**
 * Enables (unmasks) the specified IRQ lane on the i8259 PIC
 * 
 * @param {void} no paramters
 * @return {int} 0 on success, -1 on failure.
 */
int enable_irq(uint32_t irq_num) {
    if (irq_num > IRQ_SLAVE_MAX || irq_num < IRQ_MIN) return -1;
    // mask on slave PIC
    if (irq_num > IRQ_MASTER_MAX) {
        irq_num -= MAX_PINS;
        slave_mask &= ~(1 << irq_num);
        outb(slave_mask, SLAVE_DATA_PORT);
    }
    // mask on master PIC
    else {
        master_mask &= ~(1 << irq_num);
        outb(master_mask, MASTER_DATA_PORT);
    }
    return 0;
}

/**
 * Disables (masks) the specified IRQ lane on the i8259 PIC
 * 
 * @param {void} no parameters
 * @return {int} 0 on success, -1 on failure
 */
int disable_irq(uint32_t irq_num) {
    if (irq_num > IRQ_SLAVE_MAX || irq_num < IRQ_MIN) return -1;
    // unmask on slave PIC
    if (irq_num > IRQ_MASTER_MAX) {
        slave_mask |= (1 << irq_num);
        outb(slave_mask, SLAVE_DATA_PORT);
    }
    // unmask on master PIC
    else {
        master_mask |= (1 << irq_num);
        outb(master_mask, MASTER_DATA_PORT);
    }
    return 0;
}

/**
 * Send end-of-interrupt signal for the specified IRQ lane on the i8259 PIC
 * 
 * @param {void} no parameters
 * @return {void}
 * 
 */
void send_eoi(uint32_t irq_num) {
    // write EOI byte to slave and master if the IRQ was from the slave; otherwise just write to master
    if (irq_num > IRQ_MASTER_MAX) {
        outb(EOI | (irq_num - MAX_PINS), SLAVE_8259_PORT);  // write the EOI byte to port of slave
        outb(EOI | IRQ_SLAVE, MASTER_8259_PORT);            // write the EOI byte to port of master
    } else {
        outb(EOI | irq_num, MASTER_8259_PORT);  // write the EOI byte to port of master
    }
}
