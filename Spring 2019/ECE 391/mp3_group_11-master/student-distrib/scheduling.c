#include "scheduling.h"
#include "PCB.h"
#include "paging.h"
#include "syscalls.h"
#include "x86_desc.h"

void switch_tasks(uint32_t next_pid) {
    disable_scheduler();

    uint32_t curr_pid = get_running_pid();      /* Current running process */
    pcb_t* curr_pcb = get_pcb_by_pid(curr_pid); /* PCB of currently running process */
    pcb_t* next_pcb = get_pcb_by_pid(next_pid); /* PCB of next process to run */

    /* Save Function base pointer of current process */
    SAVE_TASK_FRAME(curr_pcb);

    /* Execute the first three base shells */
    if (get_available_pid() <= LAST_BASE_SHELL_PID) {
        execute((uint8_t*)"shell");
    }

    /* Restore next process' stack, program image, and set as running */
    tss.esp0 = (uint32_t)get_kern_stack_by_pid(next_pid);
    tss.ss0 = KERNEL_DS;

    set_process_page(next_pid);
    set_running_pid(next_pid);

    /* Return to next process execution point */
    RESTORE_TASK_FRAME(next_pcb);
    enable_scheduler();
    return;
}
