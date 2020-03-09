#include "PCB.h"
#include "cursor.h"
#include "lib.h"
#include "paging.h"
#include "types.h"

uint8_t hasBooted = FALSE;

/* Array to keep track of which PIDs are available */
static uint32_t available_pids[MAX_PROCESSES] = {NOT_IN_USE};
static uint32_t running_pid = 0;
static uint32_t displayed_pid = 0;

/**
 * Retrieves the currently running process on the system
 * @return {uint32_t} PID of the currently running process
 */
uint32_t get_running_pid(void) {
    uint32_t pid;
    { /* Critical Section begin */
        cli();
        pid = running_pid;
        sti();
    } /* Critical Section end */
    return pid;
}

/**
 * Retrieves the currently displayed process on the system
 * @return {uint32_t} PID of the currently displayed process
 */
uint32_t get_displayed_pid(void) {
    uint32_t pid;
    { /* Critical Section begin */
        cli();
        pid = displayed_pid;
        sti();
    } /* Critical Section end */
    return pid;
}

/**
 * Calculates the current number of processes running on the
 * system. Derived by walking the availability array.
 * 
 * @return {uint32_t} - Number of currently running processes.
 */
uint32_t get_num_processes(void) {
    uint32_t i, num = 0;
    { /* Critical Section begin */
        cli();
        for (i = 0; i < MAX_PROCESSES; i++) {
            if (available_pids[i] == IN_USE) num++;
        }
        sti();
    } /* Critical Section end */
    return num;
}

/**
 * Synchronously sets the variable keeping track of the current running
 * process on the system and routes video memory accordingly.
 * 
 * @param {uint32_t} pid - the identifier of current running process
 * @return {void} - none
 */
void set_running_pid(uint32_t pid) {
    { /* Critical Section begin */
        cli();
        running_pid = pid;
        sti();
    } /* Critical Section end */
}

/**
 * Synchronously sets the variable keeping track of the current displayed
 * process on the system and routes video memory accordingly. Additionally
 * handles swapping VGA MMO with buffered vidmem to simulate process switching
 * 
 * @param {uint32_t} next - the identifier of next process to display
 * @return {void} - none
 */
void set_displayed_pid(uint32_t next) {
    uint32_t prev;

    /* Switch to topmost process on shells */
    while (get_pcb_by_pid(next)->child_pid != ERRPID) {
        next = get_pcb_by_pid(next)->child_pid;
    }

    /* Switch displays */
    { /* Critical Section begin */
        cli();
        prev = displayed_pid;
        displayed_pid = next;
        /* Buffer and save old process' video data */
        route_process_video(prev, BUFFERED);
        memcpy((void*)PMEM_PROCESS_VIDEO_BUFFER(prev), (void*)VIDEO, 4 * KB);
        /* Enable and load new process' video data */
        route_process_video(next, DISPLAYED);
        memcpy((void*)VIDEO, (void*)PMEM_PROCESS_VIDEO_BUFFER(next), 4 * KB);
        sti();
    } /* Critical Section end */

    refresh_cursor();
}

/**
 * Returns the first available PID on the system
 * @return {uint32_t} - the first available PID of -1 if max processes has been met
 */
uint32_t get_available_pid(void) {
    int pid = 0;
    { /* Critical Section begin */
        cli();
        while (available_pids[pid] == IN_USE && pid < MAX_PROCESSES) pid++;
        if (pid >= MAX_PROCESSES) {
            /* Finding an available PID failed */
            sti();
            return ERRPID;
        } else {
            /* Was able to find an available PID */
            sti();
            return pid;
        }
    } /* Critical Section end */
}

/**
 * Returns the next Top-level PID to use for task switching
 * @return {uint32_t} - next Top-level PID
 */
uint32_t get_next_pid(void) {
    int next_pid = get_running_pid();

    while (get_pcb_by_pid(next_pid)->parent_pid != ERRPID) next_pid = get_pcb_by_pid(next_pid)->parent_pid;
    next_pid = (next_pid + 1) % MAX_BASE_SHELL;
    while (get_pcb_by_pid(next_pid)->child_pid != ERRPID) next_pid = get_pcb_by_pid(next_pid)->child_pid;

    return next_pid;
}

/**
 * Marks the availability of a given PID on the system
 * @param {uint32_t} - availability to set the PID to.
 * @return {void} - none 
 */
void set_pid_availability(uint32_t pid, uint32_t availability) {
    { /* Critical Section begin */
        cli();
        available_pids[pid] = availability;
        sti();
    } /* Critical Section end */
    return;
}

/**
 * Retrieves a pointer to the given process' PCB structure
 * 
 * @param {uint32_t} pid - the process id to retrieve PCB of
 * @return {pcb_t*} pointer to PCB structure in kernel page
 */
pcb_t* get_pcb_by_pid(uint32_t pid) {
    return (pcb_t*)(VMEM_KERNEL_END - ((pid + 1) * (8 * KB)));
}

/**
 * Retrieves a pointer to the base of the specified process'
 * kernel stack
 * 
 * @param {uint32_t} pid - process identifier to retrieve bp of
 * @return {void*} pointer to the stack base pointer
 */
void* get_kern_stack_by_pid(uint32_t pid) {
    return (void*)(VMEM_KERNEL_END - (pid * (8 * KB)));
}

/**
 * Helper function that returns -1 for usage in file_descriptor operation
 * table.
 * 
 * @return {int32_t} -1
 */
int32_t invalid_fop() {
    return -1;
}
