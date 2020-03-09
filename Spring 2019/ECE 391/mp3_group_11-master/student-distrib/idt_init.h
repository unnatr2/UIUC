#ifndef _IDT_INIT_H
#define _IDT_INIT_H

#include "types.h"

#define DPL_KERNEL 0
#define DPL_USER 3

/* Initialize the IDT */
void idt_init(void);

/* Insert gates in IDT */
void insert_idt_entry(int VEC_NUM, void* HANDLER, uint32_t DPL, uint32_t* GATE_TYPE);

#endif /* _IDT_INIT_H */
