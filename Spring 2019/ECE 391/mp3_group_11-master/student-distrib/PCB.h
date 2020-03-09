#ifndef _PCB_H_
#define _PCB_H_

#include "drivers/filesystem_driver.h"
#include "handlers/keyboard_handler.h"
#include "types.h"

#define MAX_PROCESSES 10
#define NOT_IN_USE 0
#define IN_USE 1
#define STDIN_FD 0
#define STDOUT_FD 1

#define ERRPID -1
#define SHELL_1_PID 2
#define SHELL_2_PID 1
#define SHELL_3_PID 0
#define MAX_BASE_SHELL 3
#define LAST_BASE_SHELL_PID (MAX_BASE_SHELL - 1)

#define isBaseShell (get_running_pid() <= LAST_BASE_SHELL_PID)
#define isUserProgam (!isBaseShell)

#define running_pcb() (get_pcb_by_pid(get_running_pid()))
#define display_pcb() (get_pcb_by_pid(get_displayed_pid()))

extern uint8_t hasBooted;

/* Struct Declarations */
typedef uint32_t pid_t;
typedef struct file_op {
    int32_t (*open)(const uint8_t* filename);
    int32_t (*close)(int32_t fd);
    int32_t (*read)(int32_t fd, void* buf, int32_t nbytes);
    int32_t (*write)(int32_t fd, const void* buf, int32_t nbytes);
} file_op;
typedef struct file_descriptor {
    file_op file_operations_table;
    int32_t inode;
    int32_t file_position;
    int32_t flags;
} fd_t;
typedef struct pcb {
    pid_t pid;                               /* Process identifier */
    pid_t parent_pid;                        /* Parent (spawner) process identifier */
    pid_t child_pid;                         /* Child (spawnee) process identifier, ERRPID of N/A */
    uint32_t exec_bp;                        /* base pointer (ebp) of previous kernel stack */
    uint32_t exec_sp;                        /* stack pointer (esp) of previous kernel stack */
    uint32_t scheduler_sp;                   /* ESP for switch_tasks in the scheduler */
    uint32_t scheduler_bp;                   /* EBP for switch_tasks in the scheduler */
    uint32_t prev_esp0;                      /* Parent's stack pointer */
    uint32_t prev_ss0;                       /* Parent's stack segment */
    uint8_t halt_flag;                       /* Flag for halting a process once it is scheduled */
    fd_t fd_array[8];                        /* each task can have up to 8 open files */
    uint32_t num_files;                      /* Number of files opened by the process */
    uint8_t executable[FILE_NAME_SIZE];      /* Name of the executable file */
    uint8_t arguments[KEYBOARD_BUFFER_SIZE]; /* Arguments passed to executable */
    char kbuffer[KEYBOARD_BUFFER_SIZE];      /* Process keyboard buffer */
    uint32_t kbuffer_count;                  /* Count of characters in the keyboard buffer */
    uint8_t kbuffer_enter_flag;              /* Enter flag for process kbuffer */
    uint8_t kbuffer_update_flag;             /* Flag to check if process keyboard buffer was updated */
    uint8_t kbuffer_height;                  /* Height of keyboard_buffer on screen */
    uint32_t cursor_x;                       /* x Position of the process cursor */
    uint32_t cursor_y;                       /* y Position of the process cursor */
} pcb_t;

/* Function Declarations */
int32_t invalid_fop();

pcb_t* get_pcb_by_pid(uint32_t pid);
uint32_t get_running_pid(void);
uint32_t get_displayed_pid(void);
uint32_t get_available_pid(void);
uint32_t get_num_processes(void);
void* get_kern_stack_by_pid(uint32_t pid);
uint32_t get_next_pid(void);

void set_running_pid(uint32_t pid);
void set_displayed_pid(uint32_t pid);
void set_pid_availability(uint32_t pid, uint32_t availability);

#endif /* _PCB_H_ */
