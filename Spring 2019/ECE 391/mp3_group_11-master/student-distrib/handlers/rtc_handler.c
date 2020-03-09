#include "rtc_handler.h"
#include "../drivers/rtc_driver.h"
#include "../i8259.h"
#include "../lib.h"
#include "../scheduling.h"

static uint32_t ticks = 0;

/**
 * Initializes the Real Time Clock on the system.
 * Opens the appropriate port for the RTC and enables
 * IRQ2 on the PIC
 * 
 * @return {void}
 */
void init_rtc() {
    { /* Critical Section Begin */
        cli();
        outb(REG_B, RTC_PORT);
        uint8_t prev = inb(CMOS_PORT);
        outb(REG_B, RTC_PORT);

        /* Turn on 6th bit of CMOS Register */
        outb(prev | (0x1 << 6), CMOS_PORT);

        /* Set default frequency */
        int rate = freq_log_helper(SYS_FREQUENCY);
        outb(REG_A, RTC_PORT);                        // set index to register A, disable NMI
        outb(REG_A, RTC_PORT);                        // reset index to A
        outb((prev & UPPER_MASK) | rate, CMOS_PORT);  // write only our rate to A. Note, rate is the bottom 4 bits.

        /* Enable Interrupts on PIC */
        (void)enable_irq(IRQ_RTC);
        sti();
    } /* Critical Section end */
}

/**
 * Handles hardware interrupts on the Real Time Clock.
 * Fired at the frequency set on the RTC.
 * 
 * @return {void}
 */
void handle_rtc() {
    send_eoi(IRQ_RTC);
    { /* Critical Section begin */
        cli();
        ticks++;
        outb(REG_C, RTC_PORT);
        inb(CMOS_PORT);
        sti();
    } /* Critical Section end */
}

/**
 * Blocks control at the specified frequency interval
 * 
 * @param {uint32_t} hz - frequency interval to block at
 * @return {int32_t} true or false
 */
void blockAtFreq(uint32_t hz) {
    cli();
    uint32_t start_tick = ticks;
    sti();

    /* Validate parameters */
    if (hz < MIN_FREQUENCY) hz = MIN_FREQUENCY;
    if (hz > SYS_FREQUENCY) hz = SYS_FREQUENCY;
    while (((ticks - start_tick) * hz) / SYS_FREQUENCY == 0) {
    }
}
