#ifndef INTERRUPT_HANDLERS
#define INTERRUPT_HANDLERS

/* General handler for not "well-defined" interrupts */
extern void handler_general_interrupt(void);
extern void handler_pic_irq0(void);
extern void handler_pic_irq1(void);
extern void handler_pic_irq2(void);
extern void handler_pic_irq3(void);
extern void handler_pic_irq4(void);
extern void handler_pic_irq5(void);
extern void handler_pic_irq6(void);
extern void handler_pic_irq7(void);
extern void handler_pic_irq8(void);
extern void handler_pic_irq9(void);
extern void handler_pic_irq10(void);
extern void handler_pic_irq11(void);
extern void handler_pic_irq12(void);
extern void handler_pic_irq13(void);
extern void handler_pic_irq14(void);
extern void handler_pic_irq15(void);

#endif
