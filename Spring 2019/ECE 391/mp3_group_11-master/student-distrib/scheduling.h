#ifndef _SCHEDULING_
#define _SCHEDULING_

#include "i8259.h"

void switch_tasks(uint32_t next_pid);
#define enable_scheduler() (enable_irq(IRQ_PIT));
#define disable_scheduler() (disable_irq(IRQ_PIT));
#define SAVE_TASK_FRAME(pcb) asm volatile("movl %%ebp, %0; movl %%esp, %1; "                 \
                                          : "=r"(pcb->scheduler_bp), "=r"(pcb->scheduler_sp) \
                                          :                                                  \
                                          : "memory")
#define RESTORE_TASK_FRAME(pcb) asm volatile("movl %0, %%ebp; movl %1, %%esp;"                \
                                             :                                                \
                                             : "r"(pcb->scheduler_bp), "r"(pcb->scheduler_sp) \
                                             : "memory")

#endif
