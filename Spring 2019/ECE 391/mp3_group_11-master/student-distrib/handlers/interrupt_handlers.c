#include "interrupt_handlers.h"
#include "../lib.h"
#include "keyboard_handler.h"
#include "pit_handler.h"
#include "rtc_handler.h"

/**
 * Handler for general interrupt.
 * @return {void}
 */
void handler_general_interrupt(void) {
    puts("No you don't!\n");
}

/**
 * Handler for pic IRQ0 interrupt.
 * @return {void}
 */
void handler_pic_irq0(void) {
    handle_pit();
}

/**
 * Handler for pic IRQ1 interrupt.
 * @return {void}
 */
void handler_pic_irq1(void) {
    handle_keyboard();
}

/**
 * Handler for pic IRQ2 interrupt.
 * @return {void}
 */
void handler_pic_irq2(void) {
    printf("PIC HANDLER 2 \n");
}

/**
 * Handler for pic IRQ3 interrupt.
 * @return {void}
 */
void handler_pic_irq3(void) {
    printf("PIC HANDLER 3 \n");
}

/**
 * Handler for pic IRQ4 interrupt.
 * @return {void}
 */
void handler_pic_irq4(void) {
    printf("PIC HANDLER 4 \n");
}

/**
 * Handler for pic IRQ5 interrupt.
 * @return {void}
 */
void handler_pic_irq5(void) {
    printf("PIC HANDLER 5 \n");
}

/**
 * Handler for pic IRQ6 interrupt.
 * @return {void}
 */
void handler_pic_irq6(void) {
    printf("PIC HANDLER 6 \n");
}

/**
 * Handler for pic IRQ7 interrupt.
 * @return {void}
 */
void handler_pic_irq7(void) {
    printf("PIC HANDLER 7 \n");
}

/**
 * Handler for pic IRQ8 interrupt.
 * @return {void}
 */
void handler_pic_irq8(void) {
    handle_rtc();
}

/**
 * Handler for pic IRQ9 interrupt.
 * @return {void}
 */
void handler_pic_irq9(void) {
    printf("PIC HANDLER 9 \n");
}

/**
 * Handler for pic IRQ10 interrupt.
 * @return {void}
 */
void handler_pic_irq10(void) {
    printf("PIC HANDLER 10 \n");
}

/**
 * Handler for pic IRQ11 interrupt.
 * @return {void}
 */
void handler_pic_irq11(void) {
    printf("PIC HANDLER 11 \n");
}

/**
 * Handler for pic IRQ12 interrupt.
 * @return {void}
 */
void handler_pic_irq12(void) {
    printf("PIC HANDLER 12 \n");
}

/**
 * Handler for pic IRQ13 interrupt.
 * @return {void}
 */
void handler_pic_irq13(void) {
    printf("PIC HANDLER 13 \n");
}

/**
 * Handler for pic IRQ14 interrupt.
 * @return {void}
 */
void handler_pic_irq14(void) {
    printf("PIC HANDLER 14 \n");
}

/**
 * Handler for pic IRQ15 interrupt.
 * @return {void}
 */
void handler_pic_irq15(void) {
    printf("PIC HANDLER 15 \n");
}
