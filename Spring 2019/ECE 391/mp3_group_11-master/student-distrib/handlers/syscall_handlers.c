#include "syscall_handlers.h"
#include "../PCB.h"
#include "../cursor.h"
#include "../drivers/filesystem_driver.h"
#include "../drivers/rtc_driver.h"
#include "../drivers/terminal_driver.h"
#include "../handlers/keyboard_handler.h"
#include "../i8259.h"
#include "../lib.h"
#include "../paging.h"
#include "../scheduling.h"
#include "../types.h"
#include "../x86_desc.h"
#include "context_switch.h"

#define KEEP_FISH_OUTPUT FALSE

/* Designating magic bits present in all executable binaries */
static const uint8_t execMagic[4] = {'\x7F', 'E', 'L', 'F'};

/*File operations for different files*/
static file_op std_in_operations = {terminal_open, terminal_close, terminal_read, invalid_fop};
static file_op std_out_operations = {terminal_open, terminal_close, invalid_fop, terminal_write};
static file_op rtc_operations = {rtc_open, rtc_close, rtc_read, rtc_write};
static file_op file_operations = {file_open, file_close, file_read, file_write};
static file_op dir_operations = {dir_open, dir_close, dir_read, dir_write};

/* Jump table for system call linkage to arrive here
   when new handlers are added, include them here */
void* syscall_handler_jt[SYSCALLS_MAX] = {
    syscall_handler_halt,
    syscall_handler_execute,
    syscall_handler_read,
    syscall_handler_write,
    syscall_handler_open,
    syscall_handler_close,
    syscall_handler_getargs,
    syscall_handler_vidmap,
    syscall_handler_set_handler,
    syscall_handler_sigreturn,
};

/**
 * Terminates a process, returning the specified value to its parent process.
 * The system call handler itself is responsible for expanding the 8-bit argument 
 * from BL into the 32-bit return value to the parent program’s execute system call.
 * Be careful not to return all 32 bits from EBX. This call should never return to the caller.
 * 
 * @param {uint8_t} status -- 8 bit argument in BL 
 * @return {uint32_t} never
 *
*/
asmlinkage int32_t syscall_handler_halt(uint8_t status) {
    /* Disallow Task Switching while halting */
    disable_scheduler();

    uint32_t pid = get_running_pid();          /* Halted process identifier */
    pcb_t* process = get_pcb_by_pid(pid);      /* Halted process' pcb */
    uint32_t parent_pid = process->parent_pid; /* Parent process */

    /* Close stdin and stdout separately */
    process->fd_array[STDIN_FD].file_operations_table.close(STDIN_FD);
    process->fd_array[STDOUT_FD].file_operations_table.close(STDOUT_FD);

    /* Close stdin and stdout separately */
    process->fd_array[STDIN_FD].file_operations_table.close(STDIN_FD);
    process->fd_array[STDOUT_FD].file_operations_table.close(STDOUT_FD);
    /* Close all open files */
    uint32_t fd_num;
    for (fd_num = MIN_FD; fd_num < MAX_FD; fd_num++) {
        if (process->fd_array[fd_num].flags == IN_USE) {
            syscall_handler_close(fd_num);
        }
    }

    /* Make PID available */
    set_pid_availability(pid, NOT_IN_USE);
    set_running_pid(process->parent_pid);

    /* Restore parent esp0 */
    tss.esp0 = process->prev_esp0;
    tss.ss0 = process->prev_ss0;

    if (parent_pid == ERRPID) {
        terminal_init();
        return syscall_handler_execute((uint8_t*)("shell"));
    }

    /* Remove as child from parent process */
    get_pcb_by_pid(parent_pid)->child_pid = ERRPID;

    /* Restore parent process' paging */
    set_process_page(parent_pid);

    /* If halted process is the displayed one, display parent and copy halted 
       process' video buffer into VGA MMO */
    if (get_displayed_pid() == pid) {
        set_displayed_pid(process->parent_pid);
        /* Copy video of child process into parent unless process is "fish" */
        if (strncmp((int8_t*)process->executable, (int8_t*)"fish", 4) || KEEP_FISH_OUTPUT) {
            memcpy((void*)VIDEO, (void*)PMEM_PROCESS_VIDEO_BUFFER(pid), 4 * KB);
        }
    }

    /* Restore parent cursor */
    get_pcb_by_pid(parent_pid)->cursor_x = get_pcb_by_pid(pid)->cursor_x;
    get_pcb_by_pid(parent_pid)->cursor_y = get_pcb_by_pid(pid)->cursor_y;
    refresh_cursor();

    /* If running program holds an override while halting, reset it */
    video_override_pid = ERRPID;

    /* Return back to executable execute call */
    enable_scheduler();
    return halt_ret(process->exec_sp, process->exec_bp, status);
}

/**
 * Tries to load and execute a new program, handing off the processor 
 * to the new program until it terminates. 
 * 
 * @param {uint8_t*} command -- The command is a space-separated 
 * sequence of words. The first word is the file name of the program to be
 * executed, and the rest of the command—stripped of leading spaces—should 
 * be provided to the new program on request via the getargs system call.
 * @return {int32_t}  returns -1 if the command cannot be executed, 
 * 256 if the program dies by an exception, 
 * or a value in the range 0 to 255 if the program executes a halt system call,
 * in which case the value returned is that given by the program’s call to halt.
 *
*/
asmlinkage int32_t syscall_handler_execute(uint8_t* command) {
    /* Disallow Task Switching while executing */
    disable_scheduler();

    int i;                                             /* Loop index */
    dentry_t dentry;                                   /* dentry for executable */
    uint32_t entry_addrs;                              /* pointer to first instruction of the program */
    int32_t pid = get_available_pid();                 /* PID of the new process to execute */
    int32_t parent_pid = get_running_pid();            /* Only current process can spawn execute */
    pcb_t* process = get_pcb_by_pid(pid);              /* Pointer to new process' PCB space */
    uint8_t file_header[FILE_HEADER_LEN];              /* Buffer for header bytes of executable */
    uint8_t executable[FILE_NAME_SIZE] = {NULL};       /* initialize executable name char array */
    uint8_t arguments[KEYBOARD_BUFFER_SIZE] = {NULL};  /* Arguments array */
    uint8_t _isBaseShell = pid <= LAST_BASE_SHELL_PID; /* Boolean for base shell determination */

    /* Check if max_processes has been met */
    if (pid == ERRPID) {
        printf("Failed to execute \"%s\": only %d concurrent processes are allowed.\n", command, MAX_PROCESSES);
        enable_scheduler();
        return -1;
    }

    /* If this is the first process to start, set the boot flag */
    if (hasBooted == FALSE) hasBooted = TRUE;

    /* Validate parameters */
    if (command == NULL) {
        enable_scheduler();
        return -1;
    }

    /* Extract executable name into executable */
    parseCommand(command, executable, arguments);

    /* Check if call to read_dentry_by_name is successful */
    if (read_dentry_by_name(executable, &dentry) == -1) {
        enable_scheduler();
        return -1;
    }

    /* Check that the file type is a regular file */
    if (dentry.file_type != FILE_TYPE_FILE) {
        enable_scheduler();
        return -1;
    }

    /* Extract the header of the file into file_header */
    if (read_data(dentry.inode_number, 0, file_header, FILE_HEADER_LEN) == -1) {
        enable_scheduler();
        return -1;
    }

    /* Validate file is an executable */
    if (*(uint32_t*)file_header != *(uint32_t*)execMagic) {
        enable_scheduler();
        return -1;
    }

    /* Read the entry point of the executable */
    entry_addrs = *(uint32_t*)(&file_header[EXEC_ENTRY_OFFSET]);

    /* set up paging for the executable*/
    set_process_page(pid);

    /* Load program into memory */
    read_data(dentry.inode_number, 0, (uint8_t*)(VMEM_EXEC_START + VMEM_EXEC_START_OFFSET), (4 * MB) - VMEM_EXEC_START_OFFSET);

    /* Initialize process PCB */
    *process = (pcb_t){
        /* General Process Information */
        .pid = pid,
        .parent_pid = _isBaseShell ? ERRPID : parent_pid,
        .child_pid = ERRPID,
        .prev_ss0 = tss.ss0,
        .prev_esp0 = tss.esp0,
        .halt_flag = FALSE,
        .executable[0 ... ZERO_INDEX(FILE_NAME_SIZE)] = NULL,
        .arguments[0 ... ZERO_INDEX(KEYBOARD_BUFFER_SIZE)] = NULL,
        /* Process Initial State */
        .kbuffer_height = SINGLE,
        .kbuffer_count = 0,
        .kbuffer_enter_flag = FALSE,
        .kbuffer_update_flag = FALSE,
        .kbuffer[0 ... ZERO_INDEX(KEYBOARD_BUFFER_SIZE)] = NULL,
        .cursor_x = _isBaseShell ? 0 : get_pcb_by_pid(parent_pid)->cursor_x,
        .cursor_y = _isBaseShell ? 0 : get_pcb_by_pid(parent_pid)->cursor_y,
        .num_files = 2,
        .fd_array[0 ... ZERO_INDEX(MAX_FD)] = {{NOT_IN_USE}},
        .fd_array[0 ... 1] = {.flags = IN_USE},
        .fd_array[0].file_operations_table = std_in_operations,
        .fd_array[1].file_operations_table = std_out_operations,
    };

    /* Copy arguments and command into PCB */
    for (i = 0; i < KEYBOARD_BUFFER_SIZE; i++) process->arguments[i] = arguments[i];
    for (i = 0; i < FILE_NAME_SIZE; i++) process->executable[i] = executable[i];

    // Save current stack pointer and stack segment register in TSS
    tss.esp0 = (uint32_t)get_kern_stack_by_pid(pid);
    tss.ss0 = KERNEL_DS;

    /* Mark the PID as taken in the PID availability array */
    set_pid_availability(pid, IN_USE);
    set_running_pid(pid);

    /* Initialize new process' video buffer to be contents of parent's buffer */
    memcpy((void*)PMEM_PROCESS_VIDEO_BUFFER(pid), (void*)VIDEO, 4 * KB);
    set_displayed_pid(pid);

    /* Open stdin and stdout */
    process->fd_array[STDIN_FD].file_operations_table.open(NULL);
    process->fd_array[STDOUT_FD].file_operations_table.open(NULL);

    /* Conditional processing for base/child processes */
    if (_isBaseShell) {
        terminal_init();
    } else {
        get_pcb_by_pid(parent_pid)->child_pid = pid;
    }

    /* Switch to new process */
    enable_scheduler();
    return context_switch(USER_DS, USER_CS, entry_addrs, &(process->exec_bp), &(process->exec_sp));
}

/**
 * The read system call reads data from the keyboard, a file, device (RTC), 
 * or directory. This call returns the number of bytes read. If the initial 
 * file position is at or beyond the end of file, 0 shall be returned (for 
 * normal files and the directory). In the case of the keyboard, read should
 * return data from one line that has been terminated by pressing Enter, or 
 * as much as fits in the buffer from one such line. The line returned should 
 * include the line feed character. In the case of a file, data should be read 
 * to the end of the file or the end of the buffer provided, whichever occurs 
 * sooner. In the case of reads to the directory, only the filename should be 
 * provided (as much as fits, or all 32 bytes), and subsequent reads should 
 * read from successive directory entries until the last is reached, at which 
 * point read should repeatedly return 0.
 *
 * @param {int32_t} fd - descriptor of file to read
 * @param {void*} buf - Buffer to copy bytes into
 * @param {int32_t} nbytes - number of bytes to read
 * @return {int32_t} Number of bytes read.
 */
asmlinkage int32_t syscall_handler_read(int32_t fd, void* buf, int32_t nbytes) {
    uint32_t pid = get_running_pid();
    pcb_t* process = get_pcb_by_pid(pid);

    /* Validate parameters */
    if (fd < 0 || fd > MAX_FD || buf == NULL) return -1;
    if (process->fd_array[fd].flags == NOT_IN_USE) return -1;

    /* Close the file_descriptor */
    return process->fd_array[fd].file_operations_table.read(fd, buf, nbytes);
}

/**
 * The write system call writes data to the terminal or to a device (RTC).
 * In the case of the terminal, all data should be displayed to the screen
 * immediately. In the case of the RTC, the system call should always accept
 * only a 4-byte integer specifying the interrupt rate in Hz, and should set
 * the rate of periodic interrupts accordingly. Writes to regular files should
 * always return -1 to indicate failure since the file system is read-only.
 * The call returns the number of bytes written, or -1 on failure.
 *
 * @param {int32_t} fd - file descriptor of file to write to
 * @param {const void*} buf - buffer of bytes to write
 * @param {int32_t} nbytes - Number of bytes to write from the buffer
 * @return {int32_t} Number of bytes written, or -1 on failure.
 */
asmlinkage int32_t syscall_handler_write(int32_t fd, const void* buf, int32_t nbytes) {
    uint32_t pid = get_running_pid();
    pcb_t* process = get_pcb_by_pid(pid);

    /* Validate paremeters */
    if (fd < 0 || fd > MAX_FD || buf == NULL) return -1;
    if (process->fd_array[fd].flags == NOT_IN_USE) return -1;

    /* Write to the file_descriptor */
    return process->fd_array[fd].file_operations_table.write(fd, buf, nbytes);
}

/**
 * Provides access to the file system. The call should find the directory
 * entry corresponding to the named file, allocate an unused file descriptor,
 * and set up any data necessary to handle the given type of file (directory, 
 * RTC device, or regular file). If the named file does not exist or no
 * descriptors are free, the call returns -1.
 *
 * @param {uint8_t*} filename - Name of the file to open
 * @return {int32_t} File descriptor of opened file or -1 on failure
 */
asmlinkage int32_t syscall_handler_open(uint8_t* filename) {
    int fd;
    dentry_t dir_entry;
    uint32_t pid = get_running_pid();
    pcb_t* process = get_pcb_by_pid(pid);

    /* Fill the dentry */
    if (read_dentry_by_name(filename, &dir_entry) == -1) return -1;

    /* Locate an available fd entry in the pcb fd array, return err if full */
    for (fd = MIN_FD; fd <= MAX_FD + 1; fd++) {
        if (fd == MAX_FD + 1) {
            /* All fd entries are taken */
            return -1;
        } else if (NOT_IN_USE == process->fd_array[fd].flags) {
            /* Break out with current value of fd */
            break;
        }
    }

    /* Setup open file_descriptor in pcb */
    switch (dir_entry.file_type) {
        case FILE_TYPE_RTC:
            if (rtc_open(filename) == 0) {
                process->fd_array[fd].inode = NULL;
                process->fd_array[fd].flags = IN_USE;
                process->fd_array[fd].file_position = DEFAULT_FREQUENCY;
                process->fd_array[fd].file_operations_table = rtc_operations;
                return fd;
            } else {
                return -1;
            }
        case FILE_TYPE_DIR:
            if (dir_open(filename) == 0) {
                process->fd_array[fd].inode = NULL;
                process->fd_array[fd].flags = IN_USE;
                process->fd_array[fd].file_position = FILE_START_BYTE;
                process->fd_array[fd].file_operations_table = dir_operations;
                return fd;
            } else {
                return -1;
            }
        case FILE_TYPE_FILE:
            if (file_open(filename) == 0) {
                process->fd_array[fd].inode = dir_entry.inode_number;
                process->fd_array[fd].flags = IN_USE;
                process->fd_array[fd].file_position = FILE_START_BYTE;
                process->fd_array[fd].file_operations_table = file_operations;
                return fd;
            } else {
                return -1;
            }
    }

    return -1;
}

/**
 * Closes the specified file descriptor and makes it available for return 
 * from later calls to open. 
 * 
 * @param {uint32_t} fd - file descriptor 
 * @return {int32_t} 0 on success, -1 on failure
 *
 */
asmlinkage int32_t syscall_handler_close(int32_t fd) {
    uint32_t pid = get_running_pid();
    pcb_t* process = get_pcb_by_pid(pid);

    /* Validate parameters */
    if (fd < MIN_FD || fd > MAX_FD) return -1;
    if (process->fd_array[fd].flags == NOT_IN_USE) return -1;

    /* Close the file_descriptor */
    process->fd_array[fd].flags = NOT_IN_USE;
    return process->fd_array[fd].file_operations_table.close(fd);
}

/**
 * Reads the program’s command line arguments into a user-level buffer. Obviously, 
 * these arguments must be stored as part of the task data when a new program is 
 * loaded. Here they are merely copied into user space. If there are no arguments, 
 * or if the arguments and a terminal NULL (0-byte) do not fit in the buffer, simply 
 * return -1. The shell does not request arguments, but you should probably still 
 * initialize the shell task’s argument data to the empty string.
 *
 * @param {uint8_t*} buf - user-level buffer to copy argument string into
 * @param {int32_t} nbytes - the number of bytes to copy.
 * @return {int32_t} 0 on success or -1 on failure.
 */
asmlinkage int32_t syscall_handler_getargs(uint8_t* buf, int32_t nbytes) {
    uint32_t bytes_to_copy = (nbytes < KEYBOARD_BUFFER_SIZE) ? nbytes : KEYBOARD_BUFFER_SIZE;
    uint32_t pid = get_running_pid();
    pcb_t* process = get_pcb_by_pid(pid);

    /* Validate arguments */
    if (buf == NULL) return -1;

    /* Copy the arguments to provided user-level buffer */
    memcpy(buf, &(process->arguments), bytes_to_copy);
    return 0;
}

/*
 * Maps the text-mode video memory into user space at a pre-set virtual address. 
 * Although the address returned is always the same (see the memory map section later 
 * in this handout), it should be written into the memory location provided by the 
 * caller (which must be checked for validity). If the location is invalid, the call 
 * should return -1. To avoid adding kernel-side exception handling for this sort of 
 * check, you can simply check whether the address falls within the address range 
 * covered by the single user-level page. Note that the video memory will require you 
 * to add another page mapping for the program, in this case a 4 kB page. It is not ok 
 * to simply change the permissions of the video page located < 4MB and pass that address.
 * 
 * @param {uint8_t**} screen_start - pointer in user space to copy virtual address of video memory into.
 * @return {int32_t} virtual address of video memory on success, -1 on failure.
 */

asmlinkage int32_t syscall_handler_vidmap(uint8_t** screen_start) {
    uint32_t pid = get_running_pid();
    uint32_t page_offset = pid * 4 * KB;
    uint32_t virt_vid_address = VMEM_PROCESS_VIDEO_START + page_offset;
    uint8_t pointerIsValid =
        (uint32_t)screen_start >= VMEM_EXEC_START &&
        (uint32_t)screen_start <= (VMEM_EXEC_END - sizeof(uint8_t*));

    /* Check if address is valid */
    if (!pointerIsValid) return -1;

    /* Copy virtual address of video memory */
    memcpy(screen_start, &virt_vid_address, sizeof(uint8_t*));

    /* Return address of video memory */
    return virt_vid_address;
}

/*
 * Changes the default action taken when a signal is received: the signum parameter 
 * specifies which signal’s handler to change, and the handler address points to a 
 * user-level function to be run when that signal is received. It returns 0 if the 
 * handler was successful set, and -1 on failure. If handler address is NULL (zero), 
 * the kernel should reset the action taken to be the default action.
 * 
 * @param {uint32_t} signum - Value of signal.
 * @param {void*} handler_address - Address of the handler that needs to be set.
 * @return {int32_t} 0 on success, -1 on failure.
 */
extern asmlinkage int32_t syscall_handler_set_handler(int32_t signum, void* handler_address) {
    return 0;
}

/*
 * Should copy the hardware context that was on the user-level stack back onto the 
 * processor. To find the hardware context, you will need to know the user-level value 
 * of ESP (will be saved by your system call handler) as well as the exact setup of the 
 * user-level stack frame. To copy the hardware context back onto the processor, you will 
 * actually overwrite the kernel’s copy of the process’s hardware context that was saved 
 * on the kernel stack when it handled the sigreturn system call. In this way, when the 
 * sigreturn system call handler returns to user space, the hardware context will 
 * automatically be copied back onto the processor by your return-from-kernel code that 
 * you have already written.
 * 
 * @param NULL.
 * @return {uint32_t} 0 on success, -1 on failure.
 */
extern asmlinkage int32_t syscall_handler_sigreturn(void) {
    return 0;
}

/* Helper functions */

/**
 * Extracts the executable name into the provided buffer given
 * a pointer to a NULL-terminated string, command.
 * 
 * @param {uint8_t*} command - 
 * @param {uint8_t*} buf[32] - buffer to copy executable name into
 * @return {void} - none
 */
void parseCommand(uint8_t* command, uint8_t executable_buffer[FILE_NAME_SIZE], uint8_t args_buffer[KEYBOARD_BUFFER_SIZE]) {
    int count;
    int offset;

    /* Validate parameters */
    if (command == NULL) return;

    count = 0;
    offset = 0;
    /* Ignore leading spaces */
    if (command[0] == ' ') {
        while (command[offset] == ' ') {
            offset++;
        }
    }
    /* Extract file name */
    while (command[count + offset] != ' ' && command[count + offset] != NULL) {
        executable_buffer[count] = command[count + offset];
        count++;
    }

    offset = count + offset;
    count = 0;
    /* Handle arguments */
    if (command[offset] == ' ') {
        /* Move offset to start of arguments */
        while (command[offset] == ' ') {
            offset++;
        }
        /* iterate through command and extract the rest of the args*/
        while (command[offset + count] != NULL) {
            args_buffer[count] = command[offset + count];
            count++;
        }
    }
}
